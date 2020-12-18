// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "SensorStaticMeshActor.h"
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

#pragma region Creating InstancedMesh
	InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*FString("InstancedMesh"));

	// Оптимизации
	InstancedMesh->SetCollisionProfileName(FName("NoCollision"), false);
	InstancedMesh->SetCastShadow(false);
	InstancedMesh->SetLightAttachmentsAsGroup(true);
	InstancedMesh->SetRenderCustomDepth(true);

	//InstancedMesh->AttachTo(RootComponent);
	InstancedMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
#pragma endregion

#pragma region Creating splines

	int componentCounter = 0;	// Needs for naming.

	TArray<FVector> locations = _calculator->CalculateFlatLocations(SplineResolution.X, SplineResolution.Y);
	for (FVector location : locations)
	{
		USplineComponent* splineComponent = CreateDefaultSubobject<USplineComponent>(FName("SplineComponent_" + FString::FromInt(componentCounter)));	// TODO: сплайны нужно хранить в свойствах актора (в массиве).
		splineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		splineComponent->RemoveSplinePoint(1);		// Удалить дефолтную точку.
		splineComponent->SetRelativeLocation(location * SizeMultipiler);
		SplineComponents.Add(splineComponent);
		_updateSplineComponent(splineComponent, 0);

		componentCounter++;
	}

#pragma endregion
}

void AFieldActor::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

#pragma region Updating vectors
	if (VectorMesh != NULL)	// TODO: переместить в конструктор и PostLoad.
	{
		InstancedMesh->SetStaticMesh(VectorMesh);

		_removeField();
		if (IsShowVectors == true)
		{
			_createField();
		}
	}
#pragma endregion

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
}

#if WITH_EDITOR
void AFieldActor::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.MemberProperty->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, VectorMesh))
	{
		InstancedMesh->SetStaticMesh(VectorMesh);
		//_removeField();
		//_createField();
	}
}
#endif

void AFieldActor::_createField()
{
	TArray<FVector> locations = _calculator->CalculateLocations(VectorFieldResolution);

	for (FVector location : locations)
	{
		_createSensorInstancedMesh(location * SizeMultipiler);
	}
}

void AFieldActor::_removeField()
{
	int instanceCount = InstancedMesh->GetInstanceCount();

	for (int i = instanceCount; i > 0; i--)	// нужно удалять с конца
	{
		InstancedMesh->RemoveInstance(i - 1);
	}
}

int AFieldActor::_createSensorInstancedMesh(FVector location)
{
	auto rotation = FRotator(0, 0, 0);

	/* Расчет радиуса меша */
	FVector distanse = _calculator->GetDistanceBetweenSensors(VectorFieldResolution) * SizeMultipiler;
	//double radius = (sqrt(pow(distanse, 2) + pow(distanse, 2)) / 2) * 0.25;	// Для сфер.
	//double radiusMultipiler = 0.5;
	double radiusX = (sqrt(pow(distanse.X, 2) + pow(distanse.X, 2)) / 2) * SensorMeshRadiusMultipiler;
	double radiusY = (sqrt(pow(distanse.Y, 2) + pow(distanse.Y, 2)) / 2) * SensorMeshRadiusMultipiler;
	double radiusZ = (sqrt(pow(distanse.Z, 2) + pow(distanse.Z, 2)) / 2) * SensorMeshRadiusMultipiler;
	FVector radius = FVector(radiusX, radiusY, radiusZ);
	// Пересчитаем радиус, учитывая что у сферы из StarterContent радиус равен 50:
	FVector meshRadius = radius / 50.0;

	auto transform = FTransform(rotation, location, meshRadius);
	return InstancedMesh->AddInstance(transform);
}

void AFieldActor::_updateSplineComponent(USplineComponent* splineComponent, float time)
{
	int i = 0;
	FVector splineComponentLocation = splineComponent->GetRelativeLocation();
	FVector min = _calculator->LowerLimits;
	FVector max = _calculator->UpperLimits;

	// Init loop:
	FVector offset = splineComponentLocation / SizeMultipiler;
	FVector splinePoint = (splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local) / SizeMultipiler + offset);

	FVector vel = _calculator->calc_vel(0, splinePoint.X, splinePoint.Y, splinePoint.Z);
	vel.Normalize();
	vel *= SplineCalcStep;
	FVector newSplinePoint = (splinePoint + vel) - offset;
	// /

	while (newSplinePoint.X + offset.X >= min.X && newSplinePoint.Y + offset.Y >= min.Y && newSplinePoint.Z + offset.Z >= min.Z &&
		   newSplinePoint.X + offset.X <= max.X && newSplinePoint.Y + offset.Y <= max.Y && newSplinePoint.Z + offset.Z <= max.Z &&
		   i < 5000)
	{
		splineComponent->AddSplineLocalPoint(newSplinePoint * SizeMultipiler);

		vel = _calculator->calc_vel(0, newSplinePoint.X + offset.X, newSplinePoint.Y + offset.Y, newSplinePoint.Z + offset.Z);
		vel.Normalize();
		vel *= SplineCalcStep;

		newSplinePoint = (newSplinePoint + vel);
		i++;
	}

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