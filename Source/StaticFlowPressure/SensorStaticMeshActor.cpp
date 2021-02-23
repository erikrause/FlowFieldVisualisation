// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "SensorStaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
//#include <cmath>
//#include "Calculation.h"
#include "PaperTest.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Test2.h"

AFieldActor::AFieldActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	#pragma region Creating vector field

	VectorField = CreateDefaultSubobject<UMeshVectorField>(TEXT("Mesh_vector_field"));
	VectorField->Init(&Calculator);
	VectorField->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	#pragma endregion

	// Moved to Splines classes?
	//SplineMaterial->SetScalarParameterValue(TEXT("time"), SimulationTime);
	//SplineMaterial->SetScalarParameterValue(TEXT("epsilon"), Epsilon);
	//SplineMaterial->SetScalarParameterValue(TEXT("scale"), SizeMultipiler);

	// Init Calculator:
	Calculator = CreateDefaultSubobject<UPaperTest>(TEXT("Calculator"));
	_calculator = Calculator;

	CubeCenter = (Calculator->UpperLimits + Calculator->LowerLimits) / 2;
#if WITH_EDITOR
	SetPivotOffset(CubeCenter);
#endif

#pragma region Creating cuboid surface

	//CuboidSurface = UCuboidSurface::Construct(Calculator->LowerLimits, Calculator->UpperLimits);
	CuboidSurface = CreateDefaultSubobject<UCuboidSurface>(TEXT("CuboidSurface"));
	CuboidSurface->Init(Calculator->LowerLimits, Calculator->UpperLimits);
	CuboidSurface->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
#pragma endregion


#pragma region Creating spline field

	SplineField = CreateDefaultSubobject<USplineField>(TEXT("SplineField"));
	TArray<ISplinesStartArea*> splinesStartAreas;
	splinesStartAreas.Add(CuboidSurface);
	SplineField->Init(&Calculator, splinesStartAreas);
	SplineField->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

#pragma endregion
}

void AFieldActor::PostLoad()	// Вызывается при загрузке.
{
	Super::PostLoad();
}

void AFieldActor::PostActorCreated()	// Вызывается при созданни в редакторе или запуске мира.
{
	Super::PostActorCreated();

	//initVisualisation();	//TODO: move to OnConstruction.
}

void AFieldActor::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

	SetCalculator(NewObject<UPaperTest>());	// Костыль, т.к. калькулятор сбрасывается в NULL.
	
	SetSizeMultipiler(GetActorScale3D().X);
	CubeCenter = (Calculator->UpperLimits + Calculator->LowerLimits) / 2;
#if WITH_EDITOR
	SetPivotOffset(CubeCenter * GetSizeMultipiler());
#endif

	UCuboidFace* cuboidFace = CuboidSurface->GetFaceBy(FaceAxis::XY, FacePosition::Front);
	cuboidFace->IsActivated = true;
	SplineField->UpdateSplines(0, true);
	VectorField->Revisualize();

#pragma region Updating splines
	/*
	Решение по динамическому созданию компонентов вне конструктора взято отсюда:
	https://forums.unrealengine.com/development-discussion/c-gameplay-programming/13196-construction-scripting-in-c-addstaticmeshcomponent
	и обновлений для новых версий движка:
	https://answers.unrealengine.com/questions/188898/how-can-i-add-components-in-the-onconstruction-fun.html
	*/

	// Init splines
	//SplineComponent = NewObject<USplineComponent>(this);
	//SplineComponent->RegisterComponent();
	//SplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//SplineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;		// Без этого могут быть проблемы со сборкой мусора.
	//SplineComponent->RemoveSplinePoint(1);		// Удалить дефолтную точку.

	//spline->SetRelativeLocation(FVector(0, 0, 0));
	//SplineComponent->AddSplinePoint(FVector(100, 0, 0), ESplineCoordinateSpace::Local);

	//spline->AddPoint(FSplinePoint(1.0, FVector(100, 0, 0)));
#pragma endregion
}

#pragma region Setters for uproperties

void AFieldActor::SetSimulationTime(float time)
{
	float deltaTime = time - SimulationTime;
	SimulationTime = time;	// TODO: move to Calculator.

	Calculator->Time = SimulationTime;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::SanitizeFloat(SimulationTime));
	
	VectorField->VectorMaterial->SetScalarParameterValue(TEXT("time"), SimulationTime);		// TODO: move to Revisualize()?
	SplineField->SplineMaterial->SetScalarParameterValue(TEXT("time"), SimulationTime);		// TODO: move to UpdateSpline()?

	SplineField->UpdateSplines(deltaTime);

	if (IsShowSplines)
	{
		_particleTimeCounter += deltaTime;
		_updateSplineParticles(deltaTime);

		//if (_particleTimeCounter > SplineParticlesSpawnDelay)
		//{
		//	_particleTimeCounter = 0;
		//	AddParticlesToStartPoint();
		//}
		//else if (_particleTimeCounter < 0)		// Проверка на то, что время откатили назад.
		//{
		//	_particleTimeCounter = 0;
		//}
	}
}

void AFieldActor::SetEpsilon(float epsilon)
{
	Calculator->Epsilon = epsilon;

	SplineField->SplineMaterial->SetScalarParameterValue(TEXT("epsilon"), epsilon);
	SplineField->UpdateSplines(0);

	VectorField->VectorMaterial->SetScalarParameterValue(TEXT("epsilon"), epsilon);
}

void AFieldActor::SetSizeMultipiler(float sizeMultipiler)
{
	SizeMultipiler = sizeMultipiler;
	SetActorRelativeScale3D(FVector(SizeMultipiler, SizeMultipiler, SizeMultipiler));

	SplineField->SplineMaterial->SetScalarParameterValue(TEXT("scale"), SizeMultipiler);
	VectorField->VectorMaterial->SetScalarParameterValue(TEXT("scale"), SizeMultipiler);
}

//void AFieldActor::SetIsShowVectors(bool isShowVectors)
//{
//	IsShowVectors = isShowVectors;
//
//	if (IsShowVectors)
//	{
//		//_reCreateVecotrField();
//	}
//	else
//	{
//		//VectorInstancedMesh->ClearInstances();
//	}
//	//VectorInstancedMesh->MarkRenderStateDirty();	// for debug
//}

void AFieldActor::SetLyambda(float lyambda)
{
	Calculator->Lyambda = lyambda;
	SplineField->SplineMaterial->SetScalarParameterValue(TEXT("lyambda"), lyambda);
	SplineField->UpdateSplines(0);

	VectorField->VectorMaterial->SetScalarParameterValue(TEXT("lyambda"), lyambda);
}

void AFieldActor::SetLowerLimits(FVector newLowerLimits)
{
	Calculator->LowerLimits = newLowerLimits;
	
	CuboidSurface->UpdateSurface(Calculator->LowerLimits, Calculator->UpperLimits);
	SplineField->UpdateSplines(0, true);

	VectorField->Revisualize();
}

void AFieldActor::SetUpperLimits(FVector newUpperLimits)
{
	Calculator->UpperLimits = newUpperLimits;

	CuboidSurface->UpdateSurface(Calculator->LowerLimits, Calculator->UpperLimits);
	SplineField->UpdateSplines(0, true);

	VectorField->Revisualize();
}

void AFieldActor::SetIsShowSplines(bool isShowSplines)
{
	IsShowSplines = isShowSplines;

	if (IsShowSplines)
	{
		//UpdateSpline();
	}
	else
	{
		//SplineInstancedMesh->ClearInstances();
	}
}

void AFieldActor::SetParticleSize(float particleSize)
{
	ParticleSize = particleSize;

	//for (int i = 0; i < ParticleInstancedMesh->GetInstanceCount(); i++)
	//{
	//	FTransform transform = FTransform();
	//	ParticleInstancedMesh->GetInstanceTransform(i, transform);
	//	transform.SetScale3D(FVector(0.01, 0.01, 0.01) * ParticleSize / SizeMultipiler);
	//}
	//ParticleInstancedMesh->MarkRenderStateDirty();

	AddParticlesToStartPoint();		//TODO: проверить, нужна ли эта строка?
}

#pragma endregion

#pragma region Getters for uproperties

float AFieldActor::GetSimulationTime()
{
	return SimulationTime;
}

void AFieldActor::SetCalculator(UCalculator* calculator)
{
	Calculator = calculator;
	_calculator = Calculator;

	FString name;
	Calculator->GetClass()->GetName(name);
	SplineField->SelectMaterial(name);
	SplineField->UpdateSplines(0, true);

	VectorField->Revisualize();
}

float AFieldActor::GetEpsilon()
{
	return Calculator->Epsilon;
}

float AFieldActor::GetSizeMultipiler()
{
	return GetActorScale3D().X;
}

bool AFieldActor::GetIsShowVectors()
{
	return IsShowVectors;
}

float AFieldActor::GetLyambda()
{
	return Calculator->Lyambda;
}

FVector AFieldActor::GetLowerLimits()
{
	return Calculator->LowerLimits;
}

FVector AFieldActor::GetUpperLimits()
{
	return Calculator->UpperLimits;
}

bool AFieldActor::GetIsShowSplines()
{
	return IsShowSplines;
}

float AFieldActor::GetParticleSize()
{
	return ParticleSize;
}
#pragma endregion

void AFieldActor::_updateSplineParticles(float deltaTime)
{
	////int instancesNum = ParticleInstancedMesh->GetInstanceCount();

	//FVector min = Calculator->LowerLimits;
	//FVector max = Calculator->UpperLimits;

	//ParallelFor(Splines.Num(), [this, deltaTime, min, max](int32 i)
	//{
	//	USpline* spline = Splines[i];

	//	TArray<int> particlesToDelete = TArray<int>();

	//	for (int particleId : spline->ParticleIds)
	//	{
	//		FTransform particleTransform = FTransform();
	//		ParticleInstancedMesh->GetInstanceTransform(particleId, particleTransform);
	//		FVector oldParticleLocation = particleTransform.GetLocation();
	//		// TODO: изменить по дистации на сплайне. Временное решение:
	//		FVector particleVelocity = Calculator->Calc_vel(SimulationTime, oldParticleLocation);
	//		FVector newParticleLocation = oldParticleLocation + particleVelocity * deltaTime;

	//		// Проверка на то, что частица в границах куба:
	//		if (newParticleLocation.X >= min.X && newParticleLocation.Y >= min.Y && newParticleLocation.Z >= min.Z &&
	//			newParticleLocation.X <= max.X && newParticleLocation.Y <= max.Y && newParticleLocation.Z <= max.Z)		// TODO: добавить проверку на particleVelocity.IsNearZero(); либо лимит на количество частиц.
	//		{
	//			if (particleId + 1 < spline->ParticleIds.Num())
	//			{
	//				FTransform nextParticleTransform = FTransform();
	//				ParticleInstancedMesh->GetInstanceTransform(particleId, nextParticleTransform);
	//				
	//				if (nextParticleTransform.GetLocation() == newParticleLocation)
	//				{
	//					ParticleInstancedMesh->RemoveInstance(particleId + 1);		// Если эта частица догнала следующую, то удалить следующую.
	//				}
	//			}
	//			particleTransform.SetLocation(newParticleLocation);
	//			//_mutex.Lock();
	//			ParticleInstancedMesh->UpdateInstanceTransform(particleId, particleTransform);
	//			//_mutex.Unlock();
	//		}
	//		else
	//		{
	//			//_mutex.Lock();
	//			ParticleInstancedMesh->RemoveInstance(particleId);
	//			//_mutex.Unlock();
	//			particlesToDelete.Add(particleId);
	//		}
	//		for (int particleIndex : particlesToDelete)
	//		{
	//			//spline->ParticleIds.Remove(particleIndex); TODO: debug.
	//		}
	//	}

	//}, EParallelForFlags::ForceSingleThread);		// TODO: сделать многопоток (проблемы с Remove из TArray).
	//ParticleInstancedMesh->MarkRenderStateDirty();		// Отрендерить изменения.
}

void AFieldActor::AddParticlesToStartPoint()
{
	//// Init spline particles:
	//for (USpline* spline : Splines)
	//{
	//	int id = ParticleInstancedMesh->AddInstance(FTransform(FRotator(0, 0, 0), spline->GetRelativeLocation() / SizeMultipiler, FVector(0.01, 0.01, 0.01) * ParticleSize / SizeMultipiler));
	//	spline->ParticleIds.Add(id);
	//}
}

#if WITH_EDITOR
void AFieldActor::PostEditChangeProperty(FPropertyChangedEvent& e)	// TODO: сделать сеттеры для свойств.
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.MemberProperty->GetFName() : NAME_None;

	//else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, VectorFieldResolution))
	//{
	//	SetVectorFieldResolution(VectorFieldResolution);
	//}
	//else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, VectorMeshRadiusMultipiler))
	//{
	//	SetVectorMeshRadiusMultipiler(VectorMeshRadiusMultipiler);
	//}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, EditorFlipFlopFieldsUpdate))
	{
		SplineField->UpdateSplines(0);
		VectorField->Revisualize();
	}
	//else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, IsShowVectors))
	//{
	//	SetIsShowVectors(IsShowVectors);
	//}
	//else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SplineMesh))
	//{
	//	//SplineInstancedMesh->SetStaticMesh(SplineMesh);
	//}
	//else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SplineResolution))
	//{
	//	SetSplineResolution(SplineResolution);
	//}
	//else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SplineThickness))
	//{
	//	//SetSplineThickness(SplineThickness);
	//}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, IsShowSplines))
	{
		SetIsShowSplines(IsShowSplines);
	}
	//else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SizeMultipiler))
	//{
	//	SetSizeMultipiler(SizeMultipiler);
	//}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SimulationTime))
	{
		SetSimulationTime(SimulationTime);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, ParticleSize))
	{
		SetParticleSize(ParticleSize);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, Calculator))
	{
		SetCalculator(Calculator);
	}
}
#endif

/// <summary>
/// Создаст линию тока по точкам, рассчитанным из скорости в t0. Удалит splineComponent из массива класса, если num of points = 1.
/// </summary>
/// <param name="splineComponent"></param>
/// <param name="isContinue"> Продолжить сплайн </param>
//void AFieldActor::_createSplinePoints(USplineComponent* splineComponent, bool isContinue)
//{
//	int i;
//	if (!isContinue) 
//	{
//		splineComponent->ClearSplinePoints();
//		splineComponent->AddSplineLocalPoint(FVector(0, 0, 0));		// Init point/
//		i = 0;
//	}
//	else
//	{
//		i = splineComponent->GetNumberOfSplinePoints() - 1;
//		for (i; i >= SplinePointsLimit; i--)
//		{
//			splineComponent->RemoveSplinePoint(i, false);
//		}
//	}
//	FVector splineComponentLocation = splineComponent->GetRelativeLocation();
//	FVector min = Calculator->LowerLimits;
//	FVector max = Calculator->UpperLimits;
//
//	// Loop init:
//	FVector offset = splineComponentLocation / SizeMultipiler;
//	FVector splinePoint = splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local) / SizeMultipiler + offset;
//
//	FVector vel = Calculator->Calc_vel(SimulationTime, splinePoint);
//	bool isCorrectNormalized = vel.Normalize();
//	FVector newSplinePoint = (splinePoint + vel * SplineCalcStep) - offset;
//	//
//
//	while (newSplinePoint.X + offset.X >= min.X && newSplinePoint.Y + offset.Y >= min.Y && newSplinePoint.Z + offset.Z >= min.Z &&
//		   newSplinePoint.X + offset.X <= max.X && newSplinePoint.Y + offset.Y <= max.Y && newSplinePoint.Z + offset.Z <= max.Z &&
//		   i < SplinePointsLimit &&
//		   newSplinePoint != (splineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local) / SizeMultipiler) &&	// Проверка на зацикливание кривой в пределах трех точек.
//		   isCorrectNormalized)	// Проверка на vel == 0 (например, когда t -> inf).
//	{
//		splineComponent->AddSplineLocalPoint(newSplinePoint * SizeMultipiler);
//		//splineComponent->SetSplinePointType(i + 1, ESplinePointType::Linear, true);		// TODO: check update arg.
//
//
//
//		//SplineInstancedMesh->UpdateInstanceTransform()
//		//FScopeLock(FCriticalSection())
//
//		/*splineMeshComponent->SetStaticMesh(SplineMesh);
//		splineMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
//		splineMeshComponent->SetRelativeLocation(newSplinePoint * SizeMultipiler);
//
//		// Define the spline mesh points:
//		FVector StartPoint = splineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Type::Local);
//		FVector StartTangent = splineComponent->GetTangentAtSplinePoint(i - 1, ESplineCoordinateSpace::Type::Local);
//		FVector EndPoint = splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
//		FVector EndTangent = splineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
//		splineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
//		*/
//
//
//		// Next loop prepare:
//		vel = Calculator->Calc_vel(SimulationTime, newSplinePoint + offset);
//		isCorrectNormalized = vel.Normalize();
//		newSplinePoint = (newSplinePoint + vel * SplineCalcStep);
//		i++;
//	}
//
//	// Настройка нулевой точки.
//	/*if (i == 0)
//	{
//		splineComponent->RemoveSplinePoint(0, false);
//	}
//	else
//	{
//		splineComponent->SetSplinePointType(0, ESplinePointType::Linear);
//	}*/
//	/*
//	// End of spline:
//	if (i < 1000)
//	{
//		FVector lastSplinePoint = (splineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local) / SizeMultipiler + offset);
//		vel = Calculator->calc_vel(0, lastSplinePoint.X, lastSplinePoint.Y, lastSplinePoint.Z);
//		vel.Normalize();
//		vel *= SplineCalcStep;
//		FVector outerSplineVector = vel - lastSplinePoint;	// Вектор, который вышел за границы.
//		FVector prob = lastSplinePoint + vel;
//
//		if (newSplinePoint.X + offset.X >= min.X && newSplinePoint.Y + offset.Y >= min.Y && newSplinePoint.Z + offset.Z >= min.Z)
//		{
//			outerSplineVector /= outerSplineVector.GetMin();
//		}
//		else
//		{
//			outerSplineVector /= outerSplineVector.GetMax();
//		}
//
//		newSplinePoint = (newSplinePoint + outerSplineVector);
//		splineComponent->AddSplineLocalPoint(newSplinePoint * SizeMultipiler);
//	}*/
//}

void AFieldActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFieldActor::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	//SetSimulationTime(SimulationTime + deltaSeconds);

	/*_particleTimeCounter += deltaSeconds;
	_updateSplineParticles(deltaSeconds);

	if (_particleTimeCounter > 1)
	{
		_particleTimeCounter = 0;
		_addParticlesToStartPoint();
	}

	if (SimulationTime > 5)
	{
		SimulationTime = 0;
	}*/
}

// DEPRECECATED:

/*
void ASensorStaticMeshActor::_updateSensors()
{
	// TODO: убрать if-else.
	if (IsRelativeColor)
	{
		_setRelativeColor();
	}
	else
	{
		_setAbsoluteColor();
	}
}


void ASensorStaticMeshActor::_setAbsoluteColor()
{
	for (TPair<FVector, Sensor*> mapPair : *SensorsMap)
	{
		FVector* location = &mapPair.Key;
		Sensor* sensor = mapPair.Value;

		UMaterialInstanceDynamic* dynamicMaterial = sensor->Material;

		sensor->pressure = Calculator->calc_pres(_secondsCounter, location->X, location->Y, location->Z);

		double blend = UCalculator::sigmoid(sensor->pressure * 10);
		dynamicMaterial->SetScalarParameterValue("Blend", blend);
	}
}

void ASensorStaticMeshActor::_setRelativeColor()
{
	// Init for loop.
	double minPressure = std::numeric_limits<double>::max();
	double maxPressure = std::numeric_limits<double>::lowest();

	// Первый цикл: вычисление поля давления и нахождение минимального и максимального давления.
	for (TPair<FVector, Sensor*> mapPair : *SensorsMap)
	{
		FVector* location = &mapPair.Key;
		Sensor* sensor = mapPair.Value;

		UMaterialInstanceDynamic* dynamicMaterial = sensor->Material;

		// Вычисление поля давления и его экстремумов.
		sensor->pressure = Calculator->calc_pres(_secondsCounter, location->X, location->Y, location->Z);

		if (sensor->pressure < minPressure)
		{
			minPressure = sensor->pressure;
		}
		else if (sensor->pressure > maxPressure)
		{
			maxPressure = sensor->pressure;
		}

		//double blend = Calculation::sigmoid(sensor->pressure * 10);
		//dynamicMaterial->SetScalarParameterValue("Blend", blend);
	}

	// Настройка цвета для сенсорова в зависимости от вычисленного минимального и максимального давления.
	for (TPair<FVector, Sensor*> mapPair : *SensorsMap)
	{
		Sensor* sensor = mapPair.Value;
		UMaterialInstanceDynamic* dynamicMaterial = sensor->Material;

		//double blend = Calculation::sigmoid(sensor->pressure * 10);
		double blend = (sensor->pressure - minPressure) / maxPressure;	// Перевод в диапазон [0,1].
		dynamicMaterial->SetScalarParameterValue("Blend", blend);
	}
}
*/