// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Calculator.h"
#include "PaperTest.h"
#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
//#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "SensorStaticMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class STATICFLOWPRESSURE_API AFieldActor : public AActor
{
	GENERATED_BODY()
	
public:
	AFieldActor();

	UPROPERTY(EditAnywhere, Category = "Vector calculation")
		UStaticMesh* VectorMesh;

	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		float SplineCalcStep = 0.1;

	UPROPERTY(EditAnywhere, Category = "Spline calculation")
		TArray<USplineComponent*> SplineComponents = TArray<USplineComponent*>();

	UInstancedStaticMeshComponent* InstancedMesh;

	UPROPERTY(EditAnywhere, Category = "Vector calculation", DisplayName = "Vector field resolution (number of sensors by axis)")
		FVector VectorFieldResolution = FVector(40, 40, 40);

	UPROPERTY(EditAnywhere, Category = "Spline calculation", DisplayName = "Spline resolution (number of splines by axis)")
		FVector SplineResolution = FVector(5, 5, 1);	// TODO: переделать через плотность.

	UPROPERTY(EditAnywhere, Category = "Vector calculation", DisplayName = "Vectors size (multipiler)")
		float SensorMeshRadiusMultipiler = 0.25;

	UPROPERTY(EditAnywhere, Category = "Vector calculation", DisplayName = "Show vector field")
		bool IsShowVectors = true;

	UPROPERTY(EditAnywhere, Category = "General calculation", DisplayName = "Field size (multipiler)")
		float SizeMultipiler = 200;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Events")
		void OnButtonPressed();

	void OnConstruction(const FTransform& transform) override;

protected:

	Calculator* _calculator = new PaperTest();

	// Ќужно ли проверить на WITH_EDITOR?
	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & propertyChangedEvent) override; 
#endif

	void _createField();

	void _removeField();

	int _createSensorInstancedMesh(FVector location);

	void _updateSplineComponent(USplineComponent* splineComponent, float time);
};