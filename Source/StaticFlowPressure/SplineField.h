// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spline.h"
#include "SplinesStartArea.h"
#include "SplineField.generated.h"


UCLASS()//(BlueprintType, meta = (BlueprintSpawnableComponent))
class STATICFLOWPRESSURE_API USplineField : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USplineField();

	//static USplineField* Construct(UCalculator const* const* calculator);
	void Init(UCalculator const* const* calculator, TArray<ISplinesStartArea*> splinesStartAreas, float* sizeMultipiler);

	UCalculator const* const* Calculator;	//		?move to USpline class.

	TArray<ISplinesStartArea*> SplinesStartAreas;

	void UpdateSplines(float deltaTime, bool isUpdateStartPositions = false);
	UFUNCTION(BlueprintCallable)
		void SpawnParticlesAtSplinesStart();
	UFUNCTION(BlueprintCallable)
		void SpawnParticlesAtSplinesEnd();
	UFUNCTION(BlueprintCallable)
		void FillSplineWithParticles(float distanceBetweenParticles);
	UFUNCTION(BlueprintCallable)
		void RemoveParticles();

	UPROPERTY()
		TArray<USpline*> Splines;

	UPROPERTY()
	UMaterialInstanceDynamic* SplineMaterial;

	UInstancedStaticMeshComponent* SplineInstancedMesh;
	UInstancedStaticMeshComponent* ParticleInstancedMesh;

	UPROPERTY(EditAnywhere, Category = "Visualisation")
		UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, Category = "Visualisation")
		UStaticMesh* ParticleMesh;

	TMap<FString, UMaterial*> SpllineCalculatorsAssets;
	TMap<FString, UMaterial*> _spllineCalculatorsAssetsGCDuplicate;

	void SelectMaterial(FString name);

	UPROPERTY(EditAnywhere, Category = "Visualisation")
		float SplinesThickness = 0.025;
	UPROPERTY(EditAnywhere, Category = "Visualisation")
		int SplinePointsLimit = 100;
	UPROPERTY(EditAnywhere, Category = "Visualisation")
		float SplineCalcStep = 0.1;
	UPROPERTY(EditAnywhere, Category = "Visualisation", DisplayName = "Spline resolution (number of splines by axis)")
		FIntVector Resolution = FIntVector(20, 20, 1);	// TODO: переделать через плотность.
	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		float SplineParticlesSpawnDelay = 1;
	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		float ParticleSize = 0.075;
	UPROPERTY(EditAnywhere, Category = "Visualisation")
		bool IsShowSplines = true;


#pragma region Setters

	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		void SetSplinePointsLimit(int newSplinePointsLimit);
	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		void SetSplineCalcStep(float newSplineCalcStep);
	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		void SetSplineThickness(float newSplineThickness);
	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		void SetResolution(FIntVector newResolution);
	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		void SetSplineParticlesSpawnDelay(float newSplineParticlesSpawnDelay);
	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		void SetParticleSize(float newParticleSize);

#pragma endregion


#pragma region Getters

	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		int GetSplinePointsLimit();
	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		float GetSplineCalcStep();
	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		float GetSplineThickness();
	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		FIntVector GetResolution();
	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		float GetSplineParticlesSpawnDelay();
	UFUNCTION(BlueprintCallable, Category = "Visualisation")
		float GetParticleSize();

#pragma endregion

protected:

	//TODO: move to calculator?
	void _initSplineCalculatorsAssets();

	void _addSplineCalculatorAsset(FString name);

	float _particleSpawnTimeCounter;

	float* _sizeMultipiler;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif
};
