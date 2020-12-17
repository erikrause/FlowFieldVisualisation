// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticFlowFunctionLibrary.h"
#include "Calculator.h"
//using namespace Calculation;


UStaticFlowFunctionLibrary::UStaticFlowFunctionLibrary()
{
}

TMap<FVector, AFieldActor*>  UStaticFlowFunctionLibrary::CreateSensors(UObject* WorldContextObject, FVector& scale)
{
	TMap<FVector, AFieldActor*> sensorsMap = TMap<FVector, AFieldActor*>();
	TArray<FVector> locations = Calculator::CalculateLocations(scale);
	UWorld* world = WorldContextObject->GetWorld();

	for (FVector location : locations)
	{
		AFieldActor* sensor = world->SpawnActor<AFieldActor>(AFieldActor::StaticClass(), location, FRotator(), FActorSpawnParameters());
		sensorsMap.Add(location, sensor);
	}
	
	return sensorsMap;
}