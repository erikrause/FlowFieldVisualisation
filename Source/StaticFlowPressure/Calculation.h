// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Calculation.generated.h"

//UCLASS()
class STATICFLOWPRESSURE_API Calculator
{
public:
	//GENERATED_BODY()
	Calculator();
	~Calculator();
	static double sigmoid(double x);
	virtual double calc_vel(double time, double x, double y, double z, int coor) = 0;
	virtual double calc_pres(double time, double x, double y, double z) = 0;
	static TArray<FVector*>* CalculateLocations(FVector* scale);
	static FVector GetDistanceBetweenSensors(FVector* scale);

	double epsilon;
	double lyambda;
};

/*
class STATICFLOWPRESSURE_API Calculation
{
public:
	Calculation();
	~Calculation();
};
*/