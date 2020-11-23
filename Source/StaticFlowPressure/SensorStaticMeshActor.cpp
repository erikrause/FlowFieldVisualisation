// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "SensorStaticMeshActor.h"
//#include <cmath>
#include "Calculation.h"
#include "Test1.h"
#include "Materials/MaterialInstanceDynamic.h"

ASensorStaticMeshActor::ASensorStaticMeshActor()
{
	BaseMesh = CreateDefaultSubobject<UStaticMesh>(*FString("BaseMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Arrow.Arrow"));	// TODO: убрать ссылку через строку.
	//if (SphereVisualAsset.Succeeded())
	BaseMesh = SphereVisualAsset.Object;


	static ConstructorHelpers::FObjectFinder<UMaterial> materialAsset(TEXT("/Game/SensorMaterial.SensorMaterial"));
	auto material = materialAsset.Object;
	//BaseMesh->SetMaterial(0, material);		// Error C2039 when packaging the progect.
	BaseMesh->AddMaterial(material);

	InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*FString("InstancedMesh"));
	InstancedMesh->SetStaticMesh(BaseMesh);

	// Оптимизации
	InstancedMesh->SetCollisionProfileName(FName("NoCollision"), false);
	InstancedMesh->SetCastShadow(false);
	InstancedMesh->SetLightAttachmentsAsGroup(true);
	InstancedMesh->SetRenderCustomDepth(true);		// Для outline'ов	TODO: не работает! отладить!

	//InstancedMesh->SetMaterial(0, material);
	InstancedMesh->AttachTo(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	_isStarted = false;
	
	Scale = FVector(40, 40, 40);
	IsRelativeColor = false;

	SensorsMap = new TMap<FVector, Sensor*>();
	_calculator = new Test1();
	TArray<FVector*>* locations = Calculator::CalculateLocations(&Scale);

	for (FVector* location : *locations)
	{
		int sensorMeshId = CreateSensorInstancedMesh(location);

		// Создание динамического материала.
		//auto dynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);	// Outer is old material?
		//sensorMesh->SetMaterial(0, dynamicMaterial);
		/*auto dynamicMaterial = sensorMesh->CreateDynamicMaterialInstance(0, material);
		
		// Вычисление начального цвета.
		double pressure = _calculator->calc_pres(5.0, location->X, location->Y, location->Z);
		//auto color = FLinearColor(30 * pressure, 255, 255, 0);
		double blend = 0.27 * pressure;
		dynamicMaterial->SetScalarParameterValue("Blend", blend);

		Sensor* sensor = new Sensor();
		sensor->Material = dynamicMaterial;
		sensor->Mesh = sensorMesh;
		SensorsMap->Add(*location, sensor);*/
	}
	
	StartTime = 0.0;
	_secondsCounter = StartTime;
	_updateSensors();
}

void ASensorStaticMeshActor::BeginPlay()
{
	Super::BeginPlay();

	_secondsCounter = 0.0;
	_updateSensors();
}

void ASensorStaticMeshActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (_isStarted == true)
	{
		_secondsCounter += DeltaSeconds;
		_updateSensors();
	}
}

void ASensorStaticMeshActor::OnButtonPressed()
{
	_isStarted = true;
}

#if WITH_EDITOR
void ASensorStaticMeshActor::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASensorStaticMeshActor, StartTime))
	{
		_secondsCounter = StartTime;
		_updateSensors();
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASensorStaticMeshActor, IsRelativeColor))
	{
		_updateSensors();
	}

}
#endif

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

int ASensorStaticMeshActor::CreateSensorInstancedMesh(FVector* location)
{
	double multipiler = 200;
	auto rotation = FRotator(0, 0, 0);
	FVector* scaledLocation = ScalarMultiply(*location, multipiler);

	/* Расчет радиуса меша */
	FVector distanse = Calculator::GetDistanceBetweenSensors(&Scale) * multipiler;
	//double radius = (sqrt(pow(distanse, 2) + pow(distanse, 2)) / 2) * 0.25;	// Для сфер.
	double radiusMultipiler = 0.5;
	double radiusX = (sqrt(pow(distanse.X, 2) + pow(distanse.X, 2)) / 2) * radiusMultipiler;
	double radiusY = (sqrt(pow(distanse.Y, 2) + pow(distanse.Y, 2)) / 2) * radiusMultipiler;
	double radiusZ = (sqrt(pow(distanse.Z, 2) + pow(distanse.Z, 2)) / 2) * radiusMultipiler;
	FVector radius = FVector(radiusX, radiusY, radiusZ);
	// Пересчитаем радиус, учитывая что у сферы из StarterContent радиус равен 50:
	FVector meshRadius = radius / 50.0;

	auto transform = FTransform(rotation, *scaledLocation, meshRadius);
	return InstancedMesh->AddInstance(transform);
}

/*
UInstancedStaticMeshComponent* ASensorStaticMeshActor::CreateSensorMesh(FVector* location, int number)
{
	//UInstancedStaticMeshComponent* sensorMesh = NewObject<UInstancedStaticMeshComponent>();
	UInstancedStaticMeshComponent* sensorMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*FString("Mesh" + FString::FromInt(number)));
	//sensorMesh->RegisterComponent();
	sensorMesh->SetStaticMesh(BaseMesh);
	//sensorMesh->SetFlags(RF_Transactional);
	//AddInstanceComponent(sensorMesh);
	sensorMesh->AttachTo(RootComponent);

	/*UStaticMeshComponent* sensorMesh = CreateDefaultSubobject<UStaticMeshComponent>(*FString("Mesh" + FString::FromInt(number)));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));	// TODO: убрать ссылку через строку.
	//if (SphereVisualAsset.Succeeded())
	sensorMesh->SetStaticMesh(SphereVisualAsset.Object);*//*

	FVector* scaledLocation = ScalarMultiply(*location, 200);
	sensorMesh->SetRelativeLocation(*scaledLocation);

	FVector distanse = Calculator::GetDistanceBetweenSensors(&Scale) * 200;
	//double radius = (sqrt(pow(distanse, 2) + pow(distanse, 2)) / 2) * 0.25;	// Для сфер.
	double radiusMultipiler = 0.25;
	double radiusX = (sqrt(pow(distanse.X, 2) + pow(distanse.X, 2)) / 2) * radiusMultipiler;
	double radiusY = (sqrt(pow(distanse.Y, 2) + pow(distanse.Y, 2)) / 2) * radiusMultipiler;
	double radiusZ = (sqrt(pow(distanse.Z, 2) + pow(distanse.Z, 2)) / 2) * radiusMultipiler;
	FVector radius = FVector(radiusX, radiusY, radiusZ);
	//double radius = (distanse / 2) * 0.25;	// Для куба.

	// Пересчитаем радиус, учитывая что у сферы из StarterContent радиус равен 50:
	FVector meshRadius = radius / 50.0;

	sensorMesh->SetRelativeScale3D(meshRadius);

	// Оптимизации
	sensorMesh->SetCollisionProfileName(FName("NoCollision"), false);
	sensorMesh->SetCastShadow(false);
	sensorMesh->SetLightAttachmentsAsGroup(true);

	sensorMesh->AttachTo(RootComponent);
	
	return sensorMesh;
}*/

FVector* ASensorStaticMeshActor::ScalarMultiply(FVector vector, float multipiler)
{
	vector.X *= multipiler;
	vector.Y *= multipiler;
	vector.Z *= multipiler;

	return &vector;
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
