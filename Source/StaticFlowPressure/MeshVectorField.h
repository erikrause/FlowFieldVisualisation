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

	UPROPERTY(EditAnywhere, Category = "Visualisation", DisplayName = "Vector field resolution (number of sensors by axis)")
		FIntVector Resolution = FIntVector(4, 4, 4);

	UPROPERTY(EditAnywhere, Category = "Visualisation", DisplayName = "Vectors size (multipiler)")
		float VectorMeshRadiusMultipiler = 0.5;


#pragma region Setters

	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		void SetResolution(FIntVector newVectorFieldResolution);

	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		void SetVectorMeshRadiusMultipiler(float newVectorMeshRadiusMultipiler);

#pragma endregion

#pragma region Getters

	UFUNCTION(BlueprintCallable, Category = "Vector calculation")
		FIntVector GetResolution();
	UFUNCTION(BlueprintCallable, Category = "Vector calculation")
		float GetVectorMeshRadiusMultipiler();

#pragma endregion

protected:

	TArray<FVector> _calculateVectorLocations(FIntVector resolution, bool isApplyBias = true) const;

};
