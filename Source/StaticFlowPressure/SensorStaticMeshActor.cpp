// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "SensorStaticMeshActor.h"
//#include <cmath>
#include "Calculation.h"
#include "Test1.h"
#include "Materials/MaterialInstanceDynamic.h"

ASensorStaticMeshActor::ASensorStaticMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;
	_isStarted = false;
	
	Scale = 20;
	IsRelativeColor = false;

	SensorsMap = new TMap<FVector, Sensor*>();
	_calculator = new Test1();
	TArray<FVector*>* locations = Calculator::CalculateLocations(Scale);

	int i = 0;

	for (FVector* location : *locations)
	{
		UStaticMeshComponent* sensorMesh = CreateSensorMesh(location, i);

		// Создание динамического материала.
		static ConstructorHelpers::FObjectFinder<UMaterial> materialAsset(TEXT("Material'/Game/ColorMaterial.ColorMaterial'"));		//sensor->GetMaterial(0);
		auto material = materialAsset.Object;
		//auto dynamicMaterial = UMaterialInstanceDynamic::Create(material, NULL);	// Outer is old material?
		//sensorMesh->SetMaterial(0, dynamicMaterial);
		auto dynamicMaterial = sensorMesh->CreateDynamicMaterialInstance(0, material);
		
		// Вычисление начального цвета.
		double pressure = _calculator->calc_pres(5.0, location->X, location->Y, location->Z);
		//auto color = FLinearColor(30 * pressure, 255, 255, 0);
		double blend = 0.27 * pressure;
		dynamicMaterial->SetScalarParameterValue("Blend", blend);

		Sensor* sensor = new Sensor();
		sensor->Material = dynamicMaterial;
		sensor->Mesh = sensorMesh;
		SensorsMap->Add(*location, sensor);
		i++;
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

UStaticMeshComponent* ASensorStaticMeshActor::CreateSensorMesh(FVector* location, int number)
{
	//UStaticMeshComponent* sensorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh %i"), number);
	UStaticMeshComponent* sensorMesh = CreateDefaultSubobject<UStaticMeshComponent>(*FString("Mesh" + FString::FromInt(number)));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

	//if (SphereVisualAsset.Succeeded())
	sensorMesh->SetStaticMesh(SphereVisualAsset.Object);

	FVector* scaledLocation = ScalarMultiply(*location, 200);
	sensorMesh->SetRelativeLocation(*scaledLocation);

	double distanse = Calculator::GetDistanceBetweenSensors(Scale) * 200;
	double radius = (sqrt(pow(distanse, 2) + pow(distanse, 2)) / 2) * 0.25;	// Для сфер.
	//double radius = (distanse / 2) * 0.25;	// Для куба.

	// Пересчитаем радиус, учитывая что у сферы из StarterContent радиус равен 50:
	double relativeRadius = radius / 50.0;
	double epsilonMultipiler = 1.001;
	relativeRadius *= epsilonMultipiler;

	sensorMesh->SetRelativeScale3D(FVector(relativeRadius, relativeRadius, relativeRadius));
	//sensorMesh->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));

	// Оптимизации
	sensorMesh->SetCollisionProfileName(FName("NoCollision"), false);
	sensorMesh->SetCastShadow(false);
	sensorMesh->SetLightAttachmentsAsGroup(true);

	sensorMesh->AttachTo(RootComponent);
	
	return sensorMesh;
}

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
