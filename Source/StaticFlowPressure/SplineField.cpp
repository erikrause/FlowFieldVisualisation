// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION


#include "SplineField.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
USplineField::USplineField()
{
	PrimaryComponentTick.bCanEverTick = false;

#pragma region Creating splines

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
	// Set material:
	ConstructorHelpers::FObjectFinder<UMaterial> splineMaterialAsset(TEXT("Material'/Game/SplineMaterials/SplineMaterial_Test2.SplineMaterial_Test2'"));
	//SplineMaterial = UMaterialInstanceDynamic::Create(splineMaterialAsset.Object, SplineInstancedMesh, FName("SplineMatreial"));
	//SplineInstancedMesh->SetMaterial(0, SplineMaterial);
	//SplineInstancedMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(0, SplineMaterial);
	SplineMaterial = SplineInstancedMesh->CreateDynamicMaterialInstance(0, splineMaterialAsset.Object);

	_initSplineCalculatorsAssets();

#pragma endregion
}

void USplineField::Init(UCalculator const* const* calculator, TArray<ISplinesStartArea*> splinesStartAreas)
{
	Calculator = calculator;
	SplinesStartAreas = splinesStartAreas;

	FString materialName;
	(*Calculator)->GetClass()->GetName(materialName);
	SelectMaterial(materialName);
}

void USplineField::UpdateSplines(bool isUpdateStartPositions)
{
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
				USpline* spline = USpline::Construct(location, Calculator);
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


	SplineInstancedMesh->ClearInstances();

	// Update curve:
	ParallelFor(Splines.Num(), [this](int32 i)
		{
			//_createSplinePoints(Splines[i], isContinue);
			Splines[i]->UpdateSpline(this->SplinePointsLimit, this->SplineCalcStep);
		}, EParallelForFlags::ForceSingleThread);

	// Add mesh to curve:
	for (USpline* spline : Splines)
	{
		FVector offset = spline->GetRelativeLocation();

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
	}
}

void USplineField::SelectMaterial(FString name)
{
	SplineMaterial = SplineInstancedMesh->CreateDynamicMaterialInstance(0, SpllineCalculatorsAssets[name]);
}

void USplineField::SetSplinePointsLimit(int splinePointsLimit)
{
	SplinePointsLimit = splinePointsLimit;
	UpdateSplines();
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

	UpdateSplines();
}

void USplineField::SetSplineThickness(float splineThickness)
{
	SplinesThickness = splineThickness;
	UpdateSplines();
}

void USplineField::SetResolution(FIntVector resolution)
{
	Resolution = resolution;

	UpdateSplines(true);
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
}
