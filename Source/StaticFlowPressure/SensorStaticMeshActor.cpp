// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "SensorStaticMeshActor.h"
//#include <cmath>
#include "Calculation.h"
#include "Test1.h"
#include "Materials/MaterialInstanceDynamic.h"

ASensorStaticMeshActor::ASensorStaticMeshActor()
{	
	/*
	StartTime = 0.0;
	_secondsCounter = StartTime;
	_updateSensors();*/
}

void ASensorStaticMeshActor::_init()
{
	if (BaseMesh == NULL)
	{
		BaseMesh = CreateDefaultSubobject<UStaticMesh>(*FString("BaseMesh"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/Arrow.Arrow"));	// TODO: ������ ������ ����� ������.
		//if (SphereVisualAsset.Succeeded())
		BaseMesh = SphereVisualAsset.Object;

		//static ConstructorHelpers::FObjectFinder<UMaterial> materialAsset(TEXT("/Game/SensorMaterial.SensorMaterial"));
		//auto material = materialAsset.Object;
		////BaseMesh->SetMaterial(0, material);		// Error C2039 when packaging the progect.
		//BaseMesh->AddMaterial(material);

		InstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*FString("InstancedMesh"));
		InstancedMesh->SetStaticMesh(BaseMesh);

		// �����������
		InstancedMesh->SetCollisionProfileName(FName("NoCollision"), false);
		InstancedMesh->SetCastShadow(false);
		InstancedMesh->SetLightAttachmentsAsGroup(true);
		InstancedMesh->SetRenderCustomDepth(true);		// ��� outline'��	TODO: �� ��������! ��������!

		//InstancedMesh->SetMaterial(0, material);
		InstancedMesh->AttachTo(RootComponent);
	}

	PrimaryActorTick.bCanEverTick = true;
}

void ASensorStaticMeshActor::_createField()
{
	TArray<FVector*>* locations = Calculator::CalculateLocations(&Resolution);

	for (FVector* location : *locations)
	{
		CreateSensorInstancedMesh(location);
	}
}

void ASensorStaticMeshActor::_removeField()
{
	int instanceCount = InstancedMesh->GetInstanceCount();
	for (int i = 0; i < instanceCount; i++)
	{
		InstancedMesh->RemoveInstance(i);
	}
}

int ASensorStaticMeshActor::CreateSensorInstancedMesh(FVector* location)
{
	double multipiler = 200;
	auto rotation = FRotator(0, 0, 0);
	FVector* scaledLocation = ScalarMultiply(*location, multipiler);

	/* ������ ������� ���� */
	FVector distanse = Calculator::GetDistanceBetweenSensors(&Resolution) * multipiler;
	//double radius = (sqrt(pow(distanse, 2) + pow(distanse, 2)) / 2) * 0.25;	// ��� ����.
	//double radiusMultipiler = 0.5;
	double radiusX = (sqrt(pow(distanse.X, 2) + pow(distanse.X, 2)) / 2) * SensorMeshRadiusMultipiler;
	double radiusY = (sqrt(pow(distanse.Y, 2) + pow(distanse.Y, 2)) / 2) * SensorMeshRadiusMultipiler;
	double radiusZ = (sqrt(pow(distanse.Z, 2) + pow(distanse.Z, 2)) / 2) * SensorMeshRadiusMultipiler;
	FVector radius = FVector(radiusX, radiusY, radiusZ);
	// ����������� ������, �������� ��� � ����� �� StarterContent ������ ����� 50:
	FVector meshRadius = radius / 50.0;

	auto transform = FTransform(rotation, *scaledLocation, meshRadius);
	return InstancedMesh->AddInstance(transform);
}

FVector* ASensorStaticMeshActor::ScalarMultiply(FVector vector, float multipiler)
{
	vector.X *= multipiler;
	vector.Y *= multipiler;
	vector.Z *= multipiler;

	return &vector;
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

#if WITH_EDITOR

void ASensorStaticMeshActor::PostLoad()
{
	Super::PostLoad();

	_init();
	_createField();
}
void ASensorStaticMeshActor::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;

	if ((PropertyName == GET_MEMBER_NAME_CHECKED(ASensorStaticMeshActor, Resolution)) || 
		(PropertyName == GET_MEMBER_NAME_CHECKED(ASensorStaticMeshActor, SensorMeshRadiusMultipiler)))
	{
		_removeField();
		_createField();
	}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASensorStaticMeshActor, BaseMesh))
	{
		InstancedMesh->SetStaticMesh(BaseMesh);
	}

	/*
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASensorStaticMeshActor, StartTime))
	{
		_secondsCounter = StartTime;
		_updateSensors();
	}

	
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASensorStaticMeshActor, IsRelativeColor))
	{
		_updateSensors();
	}*/

}
#endif
/*
void ASensorStaticMeshActor::_updateSensors()
{
	// TODO: ������ if-else.
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

	// ������ ����: ���������� ���� �������� � ���������� ������������ � ������������� ��������.
	for (TPair<FVector, Sensor*> mapPair : *SensorsMap)
	{
		FVector* location = &mapPair.Key;
		Sensor* sensor = mapPair.Value;

		UMaterialInstanceDynamic* dynamicMaterial = sensor->Material;

		// ���������� ���� �������� � ��� �����������.
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

	// ��������� ����� ��� ��������� � ����������� �� ������������ ������������ � ������������� ��������.
	for (TPair<FVector, Sensor*> mapPair : *SensorsMap)
	{
		Sensor* sensor = mapPair.Value;
		UMaterialInstanceDynamic* dynamicMaterial = sensor->Material;

		//double blend = Calculation::sigmoid(sensor->pressure * 10);
		double blend = (sensor->pressure - minPressure) / maxPressure;	// ������� � �������� [0,1].
		dynamicMaterial->SetScalarParameterValue("Blend", blend);
	}
}
*/