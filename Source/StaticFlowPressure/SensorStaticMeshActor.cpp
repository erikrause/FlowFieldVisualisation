// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "SensorStaticMeshActor.h"
//#include <cmath>
#include "Calculation.h"
#include "Test1.h"
#include "Materials/MaterialInstanceDynamic.h"

ASensorStaticMeshActor::ASensorStaticMeshActor()
{	
	InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*FString("InstancedMesh"));
	//InstancedMesh = NewObject<UInstancedStaticMeshComponent>();

	// Оптимизации
	InstancedMesh->SetCollisionProfileName(FName("NoCollision"), false);
	InstancedMesh->SetCastShadow(false);
	InstancedMesh->SetLightAttachmentsAsGroup(true);
	InstancedMesh->SetRenderCustomDepth(true);

	InstancedMesh->AttachTo(RootComponent);
	//PrimaryActorTick.bCanEverTick = true;
	/*
	StartTime = 0.0;
	_secondsCounter = StartTime;
	_updateSensors();*/
}

void ASensorStaticMeshActor::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

	if (VectorMesh != NULL)
	{
		InstancedMesh->SetStaticMesh(VectorMesh);

		_removeField();
		if (IsShowVectors == true)
		{
			_createField();
		}
	}
}

#if WITH_EDITOR
void ASensorStaticMeshActor::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.MemberProperty->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASensorStaticMeshActor, VectorMesh))
	{
		InstancedMesh->SetStaticMesh(VectorMesh);
		//_removeField();
		//_createField();
	}
}

#endif

void ASensorStaticMeshActor::_createField()
{
	TArray<FVector*>* locations = Calculator::CalculateLocations(&Resolution);

	for (FVector* location : *locations)
	{
		_createSensorInstancedMesh(location);
	}
}

void ASensorStaticMeshActor::_removeField()
{
	int instanceCount = InstancedMesh->GetInstanceCount();

	for (int i = instanceCount; i > 0; i--)	// нужно удалять с конца
	{
		InstancedMesh->RemoveInstance(i - 1);
	}
}

int ASensorStaticMeshActor::_createSensorInstancedMesh(FVector* location)
{
	auto rotation = FRotator(0, 0, 0);
	FVector* scaledLocation = _scalarMultiply(location, SizeMultipiler);

	/* Расчет радиуса меша */
	FVector distanse = Calculator::GetDistanceBetweenSensors(&Resolution) * SizeMultipiler;
	//double radius = (sqrt(pow(distanse, 2) + pow(distanse, 2)) / 2) * 0.25;	// Для сфер.
	//double radiusMultipiler = 0.5;
	double radiusX = (sqrt(pow(distanse.X, 2) + pow(distanse.X, 2)) / 2) * SensorMeshRadiusMultipiler;
	double radiusY = (sqrt(pow(distanse.Y, 2) + pow(distanse.Y, 2)) / 2) * SensorMeshRadiusMultipiler;
	double radiusZ = (sqrt(pow(distanse.Z, 2) + pow(distanse.Z, 2)) / 2) * SensorMeshRadiusMultipiler;
	FVector radius = FVector(radiusX, radiusY, radiusZ);
	// Пересчитаем радиус, учитывая что у сферы из StarterContent радиус равен 50:
	FVector meshRadius = radius / 50.0;

	auto transform = FTransform(rotation, *scaledLocation, meshRadius);
	return InstancedMesh->AddInstance(transform);
}

FVector* ASensorStaticMeshActor::_scalarMultiply(FVector* vector, float multipiler)
{
	vector->X *= multipiler;
	vector->Y *= multipiler;
	vector->Z *= multipiler;

	return vector;
}

void ASensorStaticMeshActor::BeginPlay()
{
	Super::BeginPlay();

	//_secondsCounter = 0.0;
	//_updateSensors();
}

void ASensorStaticMeshActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*
	if (_isStarted == true)
	{
		_secondsCounter += DeltaSeconds;
		_updateSensors();
	}*/
}

void ASensorStaticMeshActor::OnButtonPressed()
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