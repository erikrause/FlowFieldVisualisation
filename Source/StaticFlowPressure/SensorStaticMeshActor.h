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

	UPROPERTY(EditAnywhere, Category = "Visualisation")
		bool IsRelativeColor;

	UPROPERTY(EditAnywhere, Category = "Calculation")
		double StartTime;
	UPROPERTY(EditAnywhere, Category = "Calculation")
		FVector Scale;	// Масштаб по осям

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Events")
		void OnButtonPressed();

	struct Sensor
	{
		UStaticMeshComponent* Mesh;
		UMaterialInstanceDynamic* Material;
		double pressure;
	};
	TMap<FVector, Sensor*>* SensorsMap;

	//UPROPERTY(EditAnywhere, Category = "Calculation")
		//UStaticFlowFunctionLibrary* prob;

protected:
	Calculator* _calculator;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent) override; 
#endif

	bool _isStarted;

	void _updateSensors();

	UStaticMeshComponent* CreateSensorMesh(FVector* location, int number);
	FVector* ScalarMultiply(FVector vector, float multipiler);
	double _secondsCounter;

	void _setAbsoluteColor();
	void _setRelativeColor();
};