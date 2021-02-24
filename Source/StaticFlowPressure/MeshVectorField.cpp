// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshVectorField.h"

UMeshVectorField::UMeshVectorField()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

#pragma region Creating VectorInstancedMesh

	VectorInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(*FString("VectorInstancedMesh"));
	VectorInstancedMesh->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	// Set mesh asset:
	static ConstructorHelpers::FObjectFinder<UStaticMesh> vectorAsset(TEXT("/Game/Arrow.Arrow"));
	VectorInstancedMesh->SetStaticMesh(vectorAsset.Object);
	// Set material:
	static ConstructorHelpers::FObjectFinder<UMaterial> vectorMaterialAsset(TEXT("Material'/Game/SensorMaterial.SensorMaterial'"));
	VectorMaterial = UMaterialInstanceDynamic::Create(vectorMaterialAsset.Object, this, FName("VectorMatreial"));
	VectorInstancedMesh->SetMaterial(0, VectorMaterial);

	// Оптимизации:
	VectorInstancedMesh->SetCollisionProfileName(FName("NoCollision"), false);
	VectorInstancedMesh->SetCastShadow(false);
	VectorInstancedMesh->SetLightAttachmentsAsGroup(true);
	VectorInstancedMesh->SetRenderCustomDepth(true);
	//_createField();

	// Materials init:
	//VectorMaterial->SetScalarParameterValue(TEXT("time"), SimulationTime);
	//VectorMaterial->SetScalarParameterValue(TEXT("epsilon"), Epsilon);
	//VectorMaterial->SetScalarParameterValue(TEXT("scale"), SizeMultipiler);

#pragma endregion

}

void UMeshVectorField::Init(UCalculator const* const* calculator)
{
	Calculator = calculator;
}

void UMeshVectorField::Revisualize()
{
	VectorInstancedMesh->ClearInstances();

	TArray<FVector> locations = _calculateVectorLocations(Resolution);

	for (FVector location : locations)
	{
		auto rotation = FRotator(0, 0, 0);

		/* Расчет радиуса меша */
		FVector distanse = (*Calculator)->GetDistanceBetweenSensors(Resolution);	// TODO: refactor location size multipiler.
		//double radius = (sqrt(pow(distanse, 2) + pow(distanse, 2)) / 2) * 0.25;	// Для сфер.
		//double radiusMultipiler = 0.5;
		double radiusX = (sqrt(pow(distanse.X, 2) + pow(distanse.X, 2)) / 2) * VectorMeshRadiusMultipiler;
		double radiusY = (sqrt(pow(distanse.Y, 2) + pow(distanse.Y, 2)) / 2) * VectorMeshRadiusMultipiler;
		double radiusZ = (sqrt(pow(distanse.Z, 2) + pow(distanse.Z, 2)) / 2) * VectorMeshRadiusMultipiler;
		FVector radius = FVector(radiusX, radiusY, radiusZ);
		// Пересчитаем радиус, учитывая что у сферы из StarterContent радиус равен 50:
		FVector meshRadius = radius / 50.0;

		VectorInstancedMesh->AddInstance(FTransform(rotation, location, meshRadius));
	}
}

TArray<FVector> UMeshVectorField::_calculateVectorLocations(FIntVector resolution, bool isApplyBias) const
{
    // Проверка, если одна из осей <= 1 (костыль, TODO):
    FVector axisMask = FVector(1, 1, 1);
    
    for (int axis = 0; axis < 3; axis++)
    {
        if (resolution[axis] <= 1)
        {
            resolution[axis] = 2;    // минимальное значение, возможное в цикле.
            axisMask[axis] = 0;     // ось, которая <=, обнулится
        }
    }

    double tempX, tempY, tempZ;

    TArray<FVector> locations = TArray<FVector>();

    for (int i = 0; i < resolution.X; i++)
    {
        tempX = (double)i / (double)(resolution.X - 1);

        for (int j = 0; j < resolution.Y; j++)
        {
            tempY = (double)j / (double)(resolution.Y - 1);

            for (int k = 0; k < resolution.Z; k++)
            {
                tempZ = (double)k / (double)(resolution.Z - 1);

                FVector location = FVector(tempX, tempY, tempZ);
                location *= axisMask;

                if (isApplyBias)
                {
                    location = ((*Calculator)->UpperLimits - (*Calculator)->LowerLimits)* location + (*Calculator)->LowerLimits;
                }

                locations.Add(location);
            }
        }
    }

    return locations;
}

void UMeshVectorField::SetResolution(FIntVector vectorFieldResolution)
{
	Resolution = vectorFieldResolution;
	Revisualize();
}

void UMeshVectorField::SetVectorMeshRadiusMultipiler(float vectorMeshRadiusMultipiler)
{
	VectorMeshRadiusMultipiler = vectorMeshRadiusMultipiler;
	Revisualize();
}

FIntVector UMeshVectorField::GetResolution()
{
	return Resolution;
}

float UMeshVectorField::GetVectorMeshRadiusMultipiler()
{
	return VectorMeshRadiusMultipiler;
}
