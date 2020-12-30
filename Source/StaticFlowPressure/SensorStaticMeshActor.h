// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Calculator.h"
#include "PaperTest.h"
#include "Components/SplineComponent.h"
#include "Components\SplineMeshComponent.h"
#include "CoreMinimal.h"
#include <chrono>	// foor debug
using namespace std::chrono;
//#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "SensorStaticMeshActor.generated.h"


struct Spline
{
public:
	Spline(USplineComponent* splineComponent, TArray<int> particleIds)
	{
		Component = splineComponent;
		ParticleIds = particleIds;
	}
	USplineComponent* Component;

	/// <summary>
	/// Частицы, принадлежащие этому сплайну.
	/// </summary>
	TArray<int> ParticleIds;
};

/**
 * 
 */
UCLASS()
class STATICFLOWPRESSURE_API AFieldActor : public AActor
{
	GENERATED_BODY()
	
public:
	AFieldActor();

	UMaterialInstanceDynamic* VectorMaterial;

	UInstancedStaticMeshComponent* VectorInstancedMesh;

	UInstancedStaticMeshComponent* ParticleInstancedMesh;	// частица, движущаяся по сплайну.

	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		UStaticMesh* ParticleMesh;

	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		float ParticleSize = 15;

	UPROPERTY(EditAnywhere, Category = "Vector calculation")
		UStaticMesh* VectorMesh;

	UMaterialInstanceDynamic* SplineMaterial;
	UInstancedStaticMeshComponent* SplineInstancedMesh;
	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		float SplineThickness = 5;

	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		float SplineCalcStep = 0.2;
	UFUNCTION(BlueprintCallable, Category = "Spline calculation")
		void UpdateSpline(bool isContinue = false);

	TArray<Spline*> Splines = TArray<Spline*>();


	UPROPERTY(EditAnywhere, Category = "Vector calculation", DisplayName = "Vector field resolution (number of sensors by axis)")
		FIntVector VectorFieldResolution = FIntVector(30, 30, 30);

	UPROPERTY(EditAnywhere, Category = "Spline calculation", DisplayName = "Spline resolution (number of splines by axis)")
		FIntVector SplineResolution = FIntVector(20, 20, 1);	// TODO: переделать через плотность.

	UPROPERTY(EditAnywhere, Category = "Vector calculation", DisplayName = "Vectors size (multipiler)")
		float VectorMeshRadiusMultipiler = 0.5;

	UPROPERTY(EditAnywhere, Category = "Vector calculation", DisplayName = "Show vector field")
		bool IsShowVectors = true;

	UPROPERTY(EditAnywhere, Category = "Spline calculation", DisplayName = "Show splines")
		bool IsShowSplines = true;
	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		int SplinePointsLimit = 500;

	UPROPERTY(EditAnywhere, Category = "General calculation", DisplayName = "Field size (multipiler)")
		float SizeMultipiler = 200;
	UPROPERTY(EditAnywhere, Category = "General calculation")
		float Epsilon = 1;
	UPROPERTY(EditAnywhere, Category = "General calculation")
		float SimulationTime = 0;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Events")
		void OnButtonPressed();

	void OnConstruction(const FTransform& transform) override;

	void SetSplinesStart(TArray<FVector> locations);

	// Setters for uproperties:
	void SetSimulationTime(float time);
	void SetEpsilon(float epsilon);
	void SetSizeMultipiler(float sizeMultipiler);
	void SetVectorFieldResolution(FIntVector vectorFieldResolution);
	void SetVectorMeshRadiusMultipiler(float vectorMeshRadiusMultipiler);
	void SetIsShowVectors(bool isShowVectors);
	void SetSplinePointsLimit(int splinePointsLimit);
	void SetSplineResolution(FIntVector splineResolution);
	void SetSplineCalcStep(float splineCalcStep);
	void SetSplineThickness(float splineThickness);
	void SetIsShowSplines(bool isShowSplines);
	void SetParticleSize(float particleSize);

protected:

	FCriticalSection _mutex;// = FWindowsCriticalSection();	// For parallel calc.

	Calculator* _calculator = new PaperTest();

	// Нужно ли проверить на WITH_EDITOR?
	virtual void PostLoad() override;
	virtual void PostActorCreated() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & propertyChangedEvent) override; 
#endif

	void _reCreateVecotrField();

	milliseconds _time();	// for deubg;

	int _createSensorInstancedMesh(FVector location);

	void _createSplinePoints(USplineComponent* splineComponent, bool isContinue = false);

	void _initVisualisation();

	/// <summary>
	/// Обновляет позиции частиц на сплайнах.
	/// </summary>
	void _updateSplineParticles(float deltaTime);

	float _particleTimeCounter = 0;

	void _addParticlesToStartPoint();

	void _updateMaterialParameters(UMaterialInstanceDynamic* vectorMaterial);
};