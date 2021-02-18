// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spline.h"
#include "SplineField.generated.h"


UCLASS()
class STATICFLOWPRESSURE_API USplineField : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USplineField();

	static USplineField* Construct(UCalculator const* const* calculator);
	void Init(UCalculator const* const* calculator);

	UCalculator const* const* Calculator;	//		?move to USpline class.

	void UpdateSplines();

	void SetSplinesStart(TArray<FVector> startLocations);

	UPROPERTY()
		TArray<USpline*> Splines;

	UMaterialInstanceDynamic* SplineMaterial;

	UInstancedStaticMeshComponent* SplineInstancedMesh;

	UPROPERTY(EditAnywhere, Category = "Visualisation")
		UStaticMesh* SplineMesh;

	TMap<FString, UMaterial*> SpllineCalculatorsAssets;

	void SelectMaterial(FString name);

	UPROPERTY(EditAnywhere, Category = "Visualisation")
		float SplinesThickness = 0.025;

protected:

	//TODO: move to calculator?
	void _initSplineCalculatorsAssets();

	void _addSplineCalculatorAsset(FString name);
};
