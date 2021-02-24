// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION


#include "SplineField.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
USplineField::USplineField()
{
	PrimaryComponentTick.bCanEverTick = false;

#pragma region Init splines asset

	SplineInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*FString("SplineInstancedMesh"));
	SplineInstancedMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	// Оптимизации
	SplineInstancedMesh->SetCollisionProfileName(FName("NoCollision"), false);
	SplineInstancedMesh->SetCastShadow(false);
	SplineInstancedMesh->SetLightAttachmentsAsGroup(true);
	SplineInstancedMesh->SetRenderCustomDepth(true);

	// Set mesh asset:
	static ConstructorHelpers::FObjectFinder<UStaticMesh> splineAsset(TEXT("/Game/SplineMesh.SplineMesh"));
	SplineMesh = splineAsset.Object;
	SplineInstancedMesh->SetStaticMesh(SplineMesh);
	//// Set material:
	//ConstructorHelpers::FObjectFinder<UMaterial> splineMaterialAsset(TEXT("Material'/Game/SplineMaterials/SplineMaterial_Test2.SplineMaterial_Test2'"));
	//SplineMaterial = SplineInstancedMesh->CreateDynamicMaterialInstance(0, splineMaterialAsset.Object);
	_initSplineCalculatorsAssets();

#pragma endregion

///////////////////////////////////

#pragma region Init particles asset

	ParticleInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*FString("ParticleInstancedMesh"));
	ParticleInstancedMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	// Оптимизации
	ParticleInstancedMesh->SetCollisionProfileName(FName("NoCollision"), false);
	ParticleInstancedMesh->SetCastShadow(false);
	ParticleInstancedMesh->SetLightAttachmentsAsGroup(true);
	ParticleInstancedMesh->SetRenderCustomDepth(true);

	// Set mesh asset:
	static ConstructorHelpers::FObjectFinder<UStaticMesh> particleAsset(TEXT("/Game/SplineParticleMesh.SplineParticleMesh"));
	ParticleMesh = particleAsset.Object;
	ParticleInstancedMesh->SetStaticMesh(ParticleMesh);

#pragma endregion
}

void USplineField::Init(UCalculator const* const* calculator, TArray<ISplinesStartArea*> splinesStartAreas, float* sizeMultipiler)
{
	Calculator = calculator;
	_sizeMultipiler = sizeMultipiler;
	SplinesStartAreas = splinesStartAreas;

	FString materialName;
	(*Calculator)->GetClass()->GetName(materialName);
	SelectMaterial(materialName);
}

void USplineField::UpdateSplines(float deltaTime, bool isUpdateStartPositions)
{

#pragma region Update splines start position
	if (isUpdateStartPositions)
	{
		for (USpline* spline : Splines)
		{
			if (spline != NULL)
			{
				spline->DestroyComponent();
				// TODO: check for destroy particle instances.
			}
		}
		Splines.Empty();

		for (ISplinesStartArea* area : SplinesStartAreas)
		{
			TArray<FVector> locations = area->GetSplinesStartLocations(Resolution);
			for (FVector location : locations)
			{
				USpline* spline = USpline::Construct(location, Calculator, _sizeMultipiler);
				spline->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
				spline->ClearSplinePoints();		// Удаляет дефолтные точки.
				//splineComponent->bInputSplinePointsToConstructionScript = true;
				spline->SetRelativeLocation(location);
				spline->SetDrawDebug(false);
				spline->LDMaxDrawDistance = 0;		// ?
				Splines.Add(spline);
			}
		}
	}
#pragma endregion


#pragma region Spawn particles

	_particleSpawnTimeCounter += deltaTime;

	// Обработка изменения времени (play и перемотка вперед).
	while (_particleSpawnTimeCounter > SplineParticlesSpawnDelay)
	{
		_particleSpawnTimeCounter -= SplineParticlesSpawnDelay;
		SpawnParticlesAtSplinesStart();
	}
	// Обработка перемотки назад.
	while ((_particleSpawnTimeCounter < 0) && ( abs(_particleSpawnTimeCounter) > SplineParticlesSpawnDelay))
	{
		_particleSpawnTimeCounter += SplineParticlesSpawnDelay;
		SpawnParticlesAtSplinesEnd();
	}

#pragma endregion


#pragma region Update splines points

	ParallelFor(Splines.Num(), [this, deltaTime](int32 i)
		{
			//_createSplinePoints(Splines[i], isContinue);
			Splines[i]->UpdateSpline(this->SplinePointsLimit, this->SplineCalcStep);
			Splines[i]->UpdateParticles(deltaTime);
		}, EParallelForFlags::None);

#pragma endregion


#pragma region Update spline and particle visualisation

	SplineInstancedMesh->ClearInstances();
	ParticleInstancedMesh->ClearInstances();

	for (USpline* spline : Splines)
	{
		FVector offset = spline->GetRelativeLocation();

		// Update spline instanced mesh:
		for (int pointIndex = 1; pointIndex < spline->GetNumberOfSplinePoints(); pointIndex++)
		{
			FVector startPoint = spline->GetLocationAtSplinePoint(pointIndex - 1, ESplineCoordinateSpace::Local) + offset;
			FVector endPoint = spline->GetLocationAtSplinePoint(pointIndex, ESplineCoordinateSpace::Local) + offset;

			FVector direction = endPoint - startPoint;	// Includes length.
			FRotator rotation = UKismetMathLibrary::MakeRotFromX(endPoint - startPoint);
			rotation.Pitch += 90;
			float meshLength = (direction).Size();
			FVector scale = (FVector(0.01 * SplinesThickness, 0.01 * SplinesThickness, 0.01 * meshLength));	// Scale в 1 (для куба 100^3).

			//_mutex.Lock();	// Для синхронизации параллельных потоков.
			SplineInstancedMesh->AddInstance(FTransform(rotation, (endPoint), scale));
			//_mutex.Unlock();
		}

		// Update particle instanced mesh:
		for (SplineParticle* particle : spline->Particles)
		{
			FVector scale = FVector(0.01, 0.01, 0.01) * ParticleSize;
			FVector location = spline->GetLocationAtDistanceAlongSpline(particle->Distance * *_sizeMultipiler, ESplineCoordinateSpace::Local) + offset;
			FRotator rotation = FRotator(0, 0, 0);

			ParticleInstancedMesh->AddInstance(FTransform(rotation, location, scale));
		}
	}
#pragma endregion

}

void USplineField::SpawnParticlesAtSplinesStart()
{
	for (USpline* spline : Splines)
	{
		spline->SpawnParticle(0);
	}
}

void USplineField::SpawnParticlesAtSplinesEnd()
{
	for (USpline* spline : Splines)
	{
		spline->SpawnParticle(spline->GetSplineLength());
	}
}

void USplineField::FillSplineWithParticles(float distanceBetweenParticles)
{
	for (USpline* spline : Splines)
	{
		float distance = 0;
		float splineLength = spline->GetSplineLength();

		while (distance < splineLength)
		{
			spline->SpawnParticle(distance);

			distance += distanceBetweenParticles;
		}
	}
}

void USplineField::RemoveParticles()
{
	for (USpline* spline : Splines)
	{
		spline->RemoveParticles();
	}
}

void USplineField::SelectMaterial(FString name)
{
	//SplineMaterial = SplineInstancedMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, SpllineCalculatorsAssets[name]);
	//SplineMaterial = SplineInstancedMesh->CreateDynamicMaterialInstance(0, SpllineCalculatorsAssets[name]);

	SplineMaterial = UMaterialInstanceDynamic::Create(SpllineCalculatorsAssets[name], this);
	SplineInstancedMesh->SetMaterial(0, SplineMaterial);

	SplineMaterial->SetScalarParameterValue(TEXT("time"), 0);
	SplineMaterial->SetScalarParameterValue(TEXT("epsilon"), (*Calculator)->Epsilon);
	SplineMaterial->SetScalarParameterValue(TEXT("scale"), *_sizeMultipiler);
}

void USplineField::SetSplinePointsLimit(int splinePointsLimit)
{
	SplinePointsLimit = splinePointsLimit;
	UpdateSplines(0);
}

void USplineField::SetSplineCalcStep(float splineCalcStep)
{
	if (splineCalcStep > 0)
	{
		SplineCalcStep = splineCalcStep;
	}
	else
	{
		SplineCalcStep = 0.01;
	}

	UpdateSplines(0);
}

void USplineField::SetSplineThickness(float splineThickness)
{
	SplinesThickness = splineThickness;
	UpdateSplines(0);
}

void USplineField::SetResolution(FIntVector resolution)
{
	Resolution = resolution;

	UpdateSplines(0, true);
}

void USplineField::SetSplineParticlesSpawnDelay(float newSplineParticlesSpawnDelay)
{
	SplineParticlesSpawnDelay = newSplineParticlesSpawnDelay;
}

void USplineField::SetParticleSize(float newParticleSize)
{
	ParticleSize = newParticleSize;

	for (int i = 0; i < ParticleInstancedMesh->GetInstanceCount(); i++)
	{
		FTransform transform = FTransform();
		ParticleInstancedMesh->GetInstanceTransform(i, transform);
		transform.SetScale3D(FVector(0.01, 0.01, 0.01) * ParticleSize);
	}
	ParticleInstancedMesh->MarkRenderStateDirty();
}

int USplineField::GetSplinePointsLimit()
{
	return SplinePointsLimit;
}

float USplineField::GetSplineCalcStep()
{
	return SplineCalcStep;
}

float USplineField::GetSplineThickness()
{
	return SplinesThickness;
}

FIntVector USplineField::GetResolution()
{
	return Resolution;
}

float USplineField::GetSplineParticlesSpawnDelay()
{
	return SplineParticlesSpawnDelay;
}

float USplineField::GetParticleSize()
{
	return ParticleSize;
}

//TODO: move to UCalculator.
void USplineField::_initSplineCalculatorsAssets()
{
	_addSplineCalculatorAsset(FString("Test1"));
	_addSplineCalculatorAsset(FString("Test2"));
	_addSplineCalculatorAsset(FString("Test3"));
	_addSplineCalculatorAsset(FString("Test4"));
	_addSplineCalculatorAsset(FString("Test5"));
	_addSplineCalculatorAsset(FString("Test6"));
	/*_addSplineCalculatorAsset(FString("Test7"));
	_addSplineCalculatorAsset(FString("Test8"));
	_addSplineCalculatorAsset(FString("Test9"));*/
	_addSplineCalculatorAsset(FString("Test10"));
	_addSplineCalculatorAsset(FString("PaperTest"));
}

void USplineField::_addSplineCalculatorAsset(FString name)
{
	TArray< FStringFormatArg > args;
	args.Add(FStringFormatArg(name));
	args.Add(FStringFormatArg(name));
	FString materialName = FString::Format(TEXT("Material'/Game/SplineMaterials/SplineMaterial_{0}.SplineMaterial_{1}'"), args);
	ConstructorHelpers::FObjectFinder<UMaterial> splineMaterialAsset(*materialName);

	SpllineCalculatorsAssets.Add(name, splineMaterialAsset.Object);
	_spllineCalculatorsAssetsGCDuplicate.Add(name, splineMaterialAsset.Object);
}
