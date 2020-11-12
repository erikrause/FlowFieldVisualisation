// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SensorStaticMeshActor.h"
#include "StaticSensor.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StaticFlowFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class STATICFLOWPRESSURE_API UStaticFlowFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UStaticFlowFunctionLibrary();

	UFUNCTION(BlueprintCallable, Category = "StaticFlowLibrary", meta = (WorldContext = "WorldContextObject"))
	static TMap<FVector, ASensorStaticMeshActor*> CreateSensors(UObject* WorldContextObject, FVector& scale);
};
