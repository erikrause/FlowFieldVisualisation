// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticFlowFunctionLibrary.h"
#include "Calculation.h"
//using namespace Calculation;


UStaticFlowFunctionLibrary::UStaticFlowFunctionLibrary()
{
}

TMap<FVector, ASensorStaticMeshActor*>  UStaticFlowFunctionLibrary::CreateSensors(UObject* WorldContextObject, FVector& scale)
{
	TMap<FVector, ASensorStaticMeshActor*> sensorsMap = TMap<FVector, ASensorStaticMeshActor*>();
	TArray<FVector*>* locations = Calculator::CalculateLocations(&scale);
	UWorld* world = WorldContextObject->GetWorld();

	for (FVector* location : *locations)
	{
		ASensorStaticMeshActor* sensor = world->SpawnActor<ASensorStaticMeshActor>(ASensorStaticMeshActor::StaticClass(), *location, FRotator(), FActorSpawnParameters());
		sensorsMap.Add(*location, sensor);
	}
	
	return sensorsMap;
}