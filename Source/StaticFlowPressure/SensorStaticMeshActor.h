// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Calculation.h"
//#include "StaticFlowFunctionLibrary.h"
#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "SensorStaticMeshActor.generated.h"

/**
 * 
 */
UCLASS()
class STATICFLOWPRESSURE_API ASensorStaticMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	ASensorStaticMeshActor();
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StaticSensor")
	//UStaticMeshComponent* VisualMesh;

	/*UPROPERTY(EditAnywhere, Category = "Calculation")
		bool IsRelativeColor;*/

	UPROPERTY(EditAnywhere, Category = "Calculation")
		UStaticMesh* BaseMesh;

	//UPROPERTY(EditAnywhere, Category = "Calculation")
	UInstancedStaticMeshComponent* InstancedMesh;

	//TODO: UPROPERTY(EditAnywhere, Category = "Calculation")
	//	UMaterialInstance* BaseMaterial;

	//UPROPERTY(EditAnywhere, Category = "Calculation")
		//double StartTime;
	UPROPERTY(EditAnywhere, Category = "Calculation")
		FVector Resolution = FVector(5, 5, 5);	// Масштаб по осям

	UPROPERTY(EditAnywhere, Category = "Calculation")
		float SensorMeshRadiusMultipiler = 0.25;	// размер сенсоров

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Events")
		void OnButtonPressed();

	//UPROPERTY(EditAnywhere, Category = "Calculation")
		//UStaticFlowFunctionLibrary* prob;

protected:

	// Нужно ли проверить на WITH_EDITOR?
	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override; 
#endif

	//void _updateSensors();

	void _init();

	void _createField();

	void _removeField();

	int CreateSensorInstancedMesh(FVector* location);

	FVector* ScalarMultiply(FVector vector, float multipiler);
	//double _secondsCounter;

	//void _setAbsoluteColor();
	//void _setRelativeColor();
};