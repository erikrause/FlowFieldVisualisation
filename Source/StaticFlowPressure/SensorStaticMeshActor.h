// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Calculator.h"
//#include "PaperTest.h"
#include "Test1.h"
#include "Components/SplineComponent.h"
#include "Components\SplineMeshComponent.h"
#include "CoreMinimal.h"
#include <chrono>	// foor debug
using namespace std::chrono;
//#include "Engine/StaticMeshActor.h"
#include <StaticFlowPressure/CuboidSurface.h>
#include "Spline.h"
#include "SplineField.h"
#include "MeshVectorField.h"
#include "GameFramework/Actor.h"
#include "SensorStaticMeshActor.generated.h"


//struct Spline
//{
//public:
//	Spline(USplineComponent* splineComponent, TArray<int> particleIds)
//	{
//		Component = splineComponent;
//		ParticleIds = particleIds;
//	}
//	USplineComponent* Component;
//
//	/// <summary>
//	/// Частицы, принадлежащие этому сплайну.
//	/// </summary>
//	TArray<int> ParticleIds;
//};

/**
 * 
 */
UCLASS()
class STATICFLOWPRESSURE_API AFieldActor : public AActor
{
	GENERATED_BODY()
	
public:
	AFieldActor();

	UPROPERTY(EditAnywhere, Category = "Vector field visualisation")
		UMeshVectorField* VectorField;

	// TODO: move to separated class.
	//UInstancedStaticMeshComponent* ParticleInstancedMesh;	// частица, движущаяся по сплайну.

	//UPROPERTY(EditAnywhere, Category = "Spline calculation")
	//	UStaticMesh* ParticleMesh;

	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		float ParticleSize = 15;

	//UMaterialInstanceDynamic* SplineMaterial;
	//UInstancedStaticMeshComponent* SplineInstancedMesh;
	//UPROPERTY(EditAnywhere, Category = "Spline calculation")
	//	UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, Category = "Vector calculation", DisplayName = "Show vector field")
		bool IsShowVectors = true;

	UPROPERTY(EditAnywhere, Category = "Spline calculation", DisplayName = "Show splines")
		bool IsShowSplines = true;

	float SizeMultipiler = 200;

	//UPROPERTY(EditAnywhere, Category = "General calculation")
	//	float Epsilon = 1;
	UPROPERTY(EditAnywhere, Category = "General calculation")
		float SimulationTime = 0;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;

	void OnConstruction(const FTransform& transform) override;

	void AddParticlesToStartPoint();

	FVector CubeCenter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
		UCuboidSurface* CuboidSurface;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TArray<UStaticMeshComponent*> CuboidFacesMeshes;

	UPROPERTY(EditAnywhere, Category = "General calculation")
		UCalculator* Calculator;//new UTest1::UTest1();
	UCalculator* _calculator;	// Костыль для сборщика мусора.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline calsulation")
		USplineField* SplineField;

	UPROPERTY(EditAnywhere, Category = "Test")
		bool EditorFlipFlopFieldsUpdate;

#pragma region Setters for uproperties
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		void SetSimulationTime(float time);
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		void SetEpsilon(float newEpsilon);
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		void SetSizeMultipiler(float newSizeMultipiler);
	UFUNCTION(BlueprintCallable, Category = "Spline calculation")
		void SetIsShowSplines(bool newIsShowSplines);
	UFUNCTION(BlueprintCallable, Category = "Spline calculation")
		void SetParticleSize(float newParticleSize);
	UFUNCTION(BlueprintCallable, Category = "Spline calculation")
		void SetCalculator(UCalculator* newCalculator);
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		void SetLyambda(float newLyambda);
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		void SetLowerLimits(FVector newLowerLimits);
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		void SetUpperLimits(FVector newUpperLimits);
#pragma endregion

#pragma region Getters for uproperties
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		float GetSimulationTime();
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		float GetEpsilon();
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		float GetSizeMultipiler();
	UFUNCTION(BlueprintCallable, Category = "Vector calculation")
		bool GetIsShowVectors();
	UFUNCTION(BlueprintCallable, Category = "Spline calculation")
		bool GetIsShowSplines();
	UFUNCTION(BlueprintCallable, Category = "Spline calculation")
		float GetParticleSize();
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		float GetLyambda();
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		FVector GetLowerLimits();
	UFUNCTION(BlueprintCallable, Category = "General calculation")
		FVector GetUpperLimits();
#pragma endregion

protected:

	FCriticalSection _mutex;// = FWindowsCriticalSection();	// For parallel calc.

	// Нужно ли проверить на WITH_EDITOR?
	virtual void PostLoad() override;
	virtual void PostActorCreated() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & propertyChangedEvent) override; 
#endif

	//void _createSplinePoints(USplineComponent* splineComponent, bool isContinue = false);

	/// <summary>
	/// Обновляет позиции частиц на сплайнах.
	/// </summary>
	void _updateSplineParticles(float deltaTime);

	float _particleTimeCounter = 0;

	//void _updateMaterialParameters(UMaterialInstanceDynamic* vectorMaterial);
};