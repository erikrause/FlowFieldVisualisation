// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "SensorStaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
//#include <cmath>
//#include "Calculation.h"
#include "Test1.h"
#include "Materials/MaterialInstanceDynamic.h"

AFieldActor::AFieldActor()
{
	_calculator = new PaperTest();
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	#pragma region Creating VectorInstancedMesh

	VectorInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*FString("VectorInstancedMesh"));
	VectorInstancedMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Set mesh asset:
	static ConstructorHelpers::FObjectFinder<UStaticMesh> VectorAsset(TEXT("/Game/Arrow.Arrow"));
	VectorMesh = VectorAsset.Object;
	VectorInstancedMesh->SetStaticMesh(VectorMesh);
	// Set material:
	static ConstructorHelpers::FObjectFinder<UMaterial> vectorMaterialAsset(TEXT("Material'/Game/SensorMaterial.SensorMaterial'"));
	VectorMaterial = UMaterialInstanceDynamic::Create(vectorMaterialAsset.Object, this, FName("VectorMatreial"));
	_updateMaterialParameters(VectorMaterial);
	VectorInstancedMesh->SetMaterial(0, VectorMaterial);

	// Оптимизации:
	VectorInstancedMesh->SetCollisionProfileName(FName("NoCollision"), false);
	VectorInstancedMesh->SetCastShadow(false);
	VectorInstancedMesh->SetLightAttachmentsAsGroup(true);
	VectorInstancedMesh->SetRenderCustomDepth(true);
	//_createField();

	#pragma endregion

	FVector pivotOffset = (_calculator->UpperLimits + _calculator->LowerLimits) * SizeMultipiler / 2;
	SetPivotOffset(pivotOffset);

	#pragma region Creating splines

	SplineInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*FString("SplineInstancedMesh"));
	SplineInstancedMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SplineInstancedMesh->SetRelativeScale3D(FVector(SizeMultipiler, SizeMultipiler, SizeMultipiler));

	// Оптимизации
	SplineInstancedMesh->SetCollisionProfileName(FName("NoCollision"), false);
	SplineInstancedMesh->SetCastShadow(false);
	SplineInstancedMesh->SetLightAttachmentsAsGroup(true);
	SplineInstancedMesh->SetRenderCustomDepth(true);

	// Set mesh asset:
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SplineAsset(TEXT("/Game/SplineMesh.SplineMesh"));
	SplineMesh = SplineAsset.Object;
	SplineInstancedMesh->SetStaticMesh(SplineMesh);
	// Set material:
	static ConstructorHelpers::FObjectFinder<UMaterial> splineMaterialAsset(TEXT("Material'/Game/SplineMaterial.SplineMaterial'"));
	SplineMaterial = UMaterialInstanceDynamic::Create(splineMaterialAsset.Object, this, FName("SplineMatreial"));
	_updateMaterialParameters(SplineMaterial);
	SplineInstancedMesh->SetMaterial(0, SplineMaterial);

	#pragma endregion
}

/// <summary>
/// Adds splines to SplineComponents array with locations.
/// </summary>
/// <param name="locations"></param>
void AFieldActor::SetSplinesStart(TArray<FVector> locations)
{
	int componentCounter = 0;	// Needs for naming.

	for (FVector location : locations)
	{
		//USplineComponent* splineComponent = CreateDefaultSubobject<USplineComponent>(FName("SplineComponent_" + FString::FromInt(componentCounter)));
		USplineComponent* splineComponent = NewObject<USplineComponent>(this);
		splineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		splineComponent->ClearSplinePoints();		// Удалить дефолтные точки.
		//splineComponent->bInputSplinePointsToConstructionScript = true;
		splineComponent->SetRelativeLocation(location * SizeMultipiler);		// TODO: refactor location size multipiler.
		splineComponent->SetDrawDebug(false);
		splineComponent->LDMaxDrawDistance = 0;
		SplineComponents.Add(splineComponent);

		componentCounter++;
	}
}

void AFieldActor::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

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

void AFieldActor::PostLoad()
{
	Super::PostLoad();

	_initVisualisation();
}

void AFieldActor::PostActorCreated()
{
	Super::PostActorCreated();

	_initVisualisation();
}

void AFieldActor::_initVisualisation()
{
	if (IsShowVectors) // TODO: переместить в конструктор и PostLoad.
	{
		_createField();

		if (VectorMesh != NULL)
		{
			VectorInstancedMesh->SetStaticMesh(VectorMesh);
		}
	}

	if (IsShowSplines)
	{
		TArray<FVector> locations = _calculator->CalculateFlatLocations(SplineResolution.X, SplineResolution.Y);
		SetSplinesStart(locations);

		UpdateSpline();

		if (SplineMesh != NULL)
		{
			SplineInstancedMesh->SetStaticMesh(SplineMesh);
		}
	}
}

#if WITH_EDITOR
void AFieldActor::PostEditChangeProperty(FPropertyChangedEvent& e)	// TODO: сделать сеттеры для свойств.
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.MemberProperty->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, VectorMesh))
	{
		VectorInstancedMesh->SetStaticMesh(VectorMesh);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, VectorFieldResolution) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, VectorMeshRadiusMultipiler) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, IsShowVectors))
	{
		if (IsShowVectors)
		{
			_createField();
		}
		else
		{
			VectorInstancedMesh->ClearInstances();
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SplineMesh))
	{
		SplineInstancedMesh->SetStaticMesh(SplineMesh);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SplinePointsLimit))
	{
		if (IsShowSplines)
		{
			UpdateSpline(true);
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SplineResolution))
	{
		for (USplineComponent* splineComponent : SplineComponents)
		{
			splineComponent->DestroyComponent();
		}
		SplineComponents.Empty();
		TArray<FVector> locations = _calculator->CalculateFlatLocations(SplineResolution.X, SplineResolution.Y);
		SetSplinesStart(locations);
		UpdateSpline();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SplineCalcStep) ||
			 PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SplineThickness) ||
			 PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, IsShowSplines))
	{
		//_updateMaterialParameters(VectorMaterial);
		//_updateMaterialParameters(SplineMaterial);

		if (IsShowSplines)
		{
			UpdateSpline();
		}
		else
		{
			SplineInstancedMesh->ClearInstances();
		}
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SizeMultipiler) ||
			 PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, Epsilon) ||
			 PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SimulationTime))
	{
		_updateMaterialParameters(VectorMaterial);
		_updateMaterialParameters(SplineMaterial);

		for (USplineComponent* splineComponent : SplineComponents)
		{
			splineComponent->DestroyComponent();
		}
		SplineComponents.Empty();
		TArray<FVector> locations = _calculator->CalculateFlatLocations(SplineResolution.X, SplineResolution.Y);
		SetSplinesStart(locations);

		if (IsShowSplines)
		{
			UpdateSpline();
		}
		if (IsShowVectors)
		{
			_createField();
		}
	}
}
#endif

void AFieldActor::_createField()
{
	VectorInstancedMesh->ClearInstances();

	TArray<FVector> locations = _calculator->CalculateLocations(VectorFieldResolution);

	for (FVector location : locations)
	{
		_createSensorInstancedMesh(location * SizeMultipiler);
	}
}

milliseconds AFieldActor::_time()
{
	return duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());
}

int AFieldActor::_createSensorInstancedMesh(FVector location)
{
	auto rotation = FRotator(0, 0, 0);

	/* Расчет радиуса меша */
	FVector distanse = _calculator->GetDistanceBetweenSensors(VectorFieldResolution) * SizeMultipiler;	// TODO: refactor location size multipiler.
	//double radius = (sqrt(pow(distanse, 2) + pow(distanse, 2)) / 2) * 0.25;	// Для сфер.
	//double radiusMultipiler = 0.5;
	double radiusX = (sqrt(pow(distanse.X, 2) + pow(distanse.X, 2)) / 2) * VectorMeshRadiusMultipiler;
	double radiusY = (sqrt(pow(distanse.Y, 2) + pow(distanse.Y, 2)) / 2) * VectorMeshRadiusMultipiler;
	double radiusZ = (sqrt(pow(distanse.Z, 2) + pow(distanse.Z, 2)) / 2) * VectorMeshRadiusMultipiler;
	FVector radius = FVector(radiusX, radiusY, radiusZ);
	// Пересчитаем радиус, учитывая что у сферы из StarterContent радиус равен 50:
	FVector meshRadius = radius / 50.0;

	auto transform = FTransform(rotation, location, meshRadius);
	return VectorInstancedMesh->AddInstance(transform);
}

/// <summary>
/// Создаст линию тока по точкам, рассчитанным из скорости в t0. Удалит splineComponent из массива класса, если num of points = 1.
/// </summary>
/// <param name="splineComponent"></param>
/// <param name="time"> t0 </param>
/// <param name="isContinue"> Продолжить сплайн </param>
void AFieldActor::_createSplinePoints(USplineComponent* splineComponent, bool isContinue)
{
	int i;
	if (!isContinue) 
	{
		splineComponent->ClearSplinePoints();
		splineComponent->AddSplineLocalPoint(FVector(0, 0, 0));		// Init point/
		i = 0;
	}
	else
	{
		i = splineComponent->GetNumberOfSplinePoints() - 1;
		for (i; i >= SplinePointsLimit; i--)
		{
			splineComponent->RemoveSplinePoint(i, false);
		}
	}
	FVector splineComponentLocation = splineComponent->GetRelativeLocation();
	FVector min = _calculator->LowerLimits;
	FVector max = _calculator->UpperLimits;

	// Loop init:
	FVector offset = splineComponentLocation / SizeMultipiler;
	FVector splinePoint = splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local) / SizeMultipiler + offset;

	FVector vel = _calculator->calc_vel(SimulationTime, splinePoint.X, splinePoint.Y, splinePoint.Z);
	bool isCorrectNormalized = vel.Normalize();
	FVector newSplinePoint = (splinePoint + vel * SplineCalcStep) - offset;
	//

	while (newSplinePoint.X + offset.X >= min.X && newSplinePoint.Y + offset.Y >= min.Y && newSplinePoint.Z + offset.Z >= min.Z &&
		   newSplinePoint.X + offset.X <= max.X && newSplinePoint.Y + offset.Y <= max.Y && newSplinePoint.Z + offset.Z <= max.Z &&
		   i < SplinePointsLimit &&
		   newSplinePoint != (splineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local) / SizeMultipiler) &&	// Проверка на зацикливание кривой в пределах трех точек.
		   isCorrectNormalized)	// Проверка на vel == 0 (например, когда t -> inf).
	{
		splineComponent->AddSplineLocalPoint(newSplinePoint * SizeMultipiler);
		//splineComponent->SetSplinePointType(i + 1, ESplinePointType::Linear, true);		// TODO: check update arg.



		//SplineInstancedMesh->UpdateInstanceTransform()
		//FScopeLock(FCriticalSection())

		/*splineMeshComponent->SetStaticMesh(SplineMesh);
		splineMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		splineMeshComponent->SetRelativeLocation(newSplinePoint * SizeMultipiler);

		// Define the spline mesh points:
		FVector StartPoint = splineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Type::Local);
		FVector StartTangent = splineComponent->GetTangentAtSplinePoint(i - 1, ESplineCoordinateSpace::Type::Local);
		FVector EndPoint = splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
		FVector EndTangent = splineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
		splineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		*/


		// Next loop prepare:
		vel = _calculator->calc_vel(SimulationTime, newSplinePoint.X + offset.X, newSplinePoint.Y + offset.Y, newSplinePoint.Z + offset.Z);
		isCorrectNormalized = vel.Normalize();
		newSplinePoint = (newSplinePoint + vel * SplineCalcStep);
		i++;
	}

	// Настройка нулевой точки.
	/*if (i == 0)
	{
		splineComponent->RemoveSplinePoint(0, false);
	}
	else
	{
		splineComponent->SetSplinePointType(0, ESplinePointType::Linear);
	}*/
	/*
	// End of spline:
	if (i < 1000)
	{
		FVector lastSplinePoint = (splineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local) / SizeMultipiler + offset);
		vel = _calculator->calc_vel(0, lastSplinePoint.X, lastSplinePoint.Y, lastSplinePoint.Z);
		vel.Normalize();
		vel *= SplineCalcStep;
		FVector outerSplineVector = vel - lastSplinePoint;	// Вектор, который вышел за границы.
		FVector prob = lastSplinePoint + vel;

		if (newSplinePoint.X + offset.X >= min.X && newSplinePoint.Y + offset.Y >= min.Y && newSplinePoint.Z + offset.Z >= min.Z)
		{
			outerSplineVector /= outerSplineVector.GetMin();
		}
		else
		{
			outerSplineVector /= outerSplineVector.GetMax();
		}

		newSplinePoint = (newSplinePoint + outerSplineVector);
		splineComponent->AddSplineLocalPoint(newSplinePoint * SizeMultipiler);
	}*/
}

void AFieldActor::_updateMaterialParameters(UMaterialInstanceDynamic* material)
{
	material->SetScalarParameterValue(TEXT("scale"), SizeMultipiler);
	material->SetScalarParameterValue(TEXT("epsilon"), Epsilon);
	material->SetScalarParameterValue(TEXT("time"), SimulationTime);		// TODO: вынести time отдельно? Влиет ли на проиводительность дублирование scale и epsilon?
}

void AFieldActor::UpdateSpline(bool isContinue)
{
	SplineInstancedMesh->ClearInstances();

	// Update curve:
	ParallelFor(SplineComponents.Num(), [this, isContinue](int32 i)
	{
		_createSplinePoints(SplineComponents[i], isContinue);
	}, EParallelForFlags::ForceSingleThread);

	// Add mesh to curve:
	for (USplineComponent* splineComponent : SplineComponents)
	{
		FVector offset = splineComponent->GetRelativeLocation() / SizeMultipiler;

		for (int pointIndex = 1; pointIndex < splineComponent->GetNumberOfSplinePoints(); pointIndex++)
		{
			FVector startPoint = splineComponent->GetLocationAtSplinePoint(pointIndex - 1, ESplineCoordinateSpace::Local) / SizeMultipiler + offset;
			FVector endPoint = splineComponent->GetLocationAtSplinePoint(pointIndex, ESplineCoordinateSpace::Local) / SizeMultipiler + offset;

			FVector direction = endPoint - startPoint;	// Includes length.
			FRotator rotation = UKismetMathLibrary::MakeRotFromX(endPoint - startPoint);
			rotation.Pitch += 90;
			float meshLength = (direction).Size() * SizeMultipiler;
			FVector scale = (FVector(0.01 * SplineThickness, 0.01 * SplineThickness, 0.01 * meshLength)) / SizeMultipiler;	// Scale в 1 (для куба 100^3).

			//_mutex.Lock();	// Для синхронизации параллельных потоков.
			SplineInstancedMesh->AddInstance(FTransform(rotation, (endPoint), scale));
			//_mutex.Unlock();
		}
	}

	/*
	for (USplineComponent* splineComponent : SplineComponents)
	{
		_createSplinePoints(splineComponent, 0);
	}*/
	int prob = 0;
}

void AFieldActor::BeginPlay()
{
	Super::BeginPlay();

	//_secondsCounter = 0.0;
	//_updateSensors();
}

void AFieldActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*
	if (_isStarted == true)
	{
		_secondsCounter += DeltaSeconds;
		_updateSensors();
	}*/
}

void AFieldActor::OnButtonPressed()
{
	//_isStarted = true;
}

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

		sensor->pressure = _calculator->calc_pres(_secondsCounter, location->X, location->Y, location->Z);

		double blend = Calculator::sigmoid(sensor->pressure * 10);
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
		sensor->pressure = _calculator->calc_pres(_secondsCounter, location->X, location->Y, location->Z);

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