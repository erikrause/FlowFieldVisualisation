// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Calculator.h"
#include "Components/SceneComponent.h"
#include "MeshVectorField.generated.h"


UCLASS()
class STATICFLOWPRESSURE_API UMeshVectorField : public USceneComponent
{
	GENERATED_BODY()

public:	
	UMeshVectorField();

	void Init(UCalculator const* const* calculator);

	UCalculator const* const* Calculator;

	UInstancedStaticMeshComponent* VectorInstancedMesh;

	UMaterialInstanceDynamic* VectorMaterial;

	void Revisualize();

	UPROPERTY(EditAnywhere, Category = "Vector calculation", DisplayName = "Vector field resolution (number of sensors by axis)")
		FIntVector Resolution = FIntVector(4, 4, 4);

	UPROPERTY(EditAnywhere, Category = "Vector calculation", DisplayName = "Vectors size (multipiler)")
		float VectorMeshRadiusMultipiler = 0.5;
};
