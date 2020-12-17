// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Calculation.h"
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

	UPROPERTY(EditAnywhere, Category = "Calculation")
		UStaticMesh* VectorMesh;

	UInstancedStaticMeshComponent* InstancedMesh;
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, Category = "Calculation", DisplayName = "Vector field resolution (number of sensors by axis)")
		FVector VectorFieldResolution = FVector(40, 40, 40);

	UPROPERTY(EditAnywhere, Category = "Calculation", DisplayName = "Spline resolution (number of splines by axis)")
		FVector SplineResolution = FVector(20, 20, 1);	// TODO: переделать через плотность.

	UPROPERTY(EditAnywhere, Category = "Calculation", DisplayName = "Vectors size (multipiler)")
		float SensorMeshRadiusMultipiler = 0.25;

	UPROPERTY(EditAnywhere, Category = "Calculation", DisplayName = "Show vector field")
		bool IsShowVectors = true;

	UPROPERTY(EditAnywhere, Category = "Calculation", DisplayName = "Vector field size (multipiler)")
		float SizeMultipiler = 200;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Events")
		void OnButtonPressed();

	void OnConstruction(const FTransform& transform) override;

protected:

	// Ќужно ли проверить на WITH_EDITOR?
	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & propertyChangedEvent) override; 
#endif

	void _createField();

	void _removeField();

	int _createSensorInstancedMesh(FVector location);

	FVector _scalarMultiply(FVector vector, float multipiler);
};