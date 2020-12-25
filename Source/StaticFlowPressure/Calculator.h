// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Calculation.generated.h"
/*
struct Bounds
{
	Bounds(FVector minimums, FVector maximums)
	{
		Minimums = minimums;
		Maximums = maximums;
	}

	FVector Minimums;
	FVector Maximums;

	// лежит ли точка в пределах границ.
	int operator==(const FVector point)
	{
		return (point.X >= Minimums.X && point.Y >= Minimums.Y && point.Z >= Minimums.Z && 
				point.X <= Maximums.X && point.Y <= Maximums.Y && point.Z <= Maximums.Z);
	}
};*/

//UCLASS()
class STATICFLOWPRESSURE_API Calculator
{
public:
	//GENERATED_BODY()
	Calculator();
	~Calculator();
	static double sigmoid(double x);
	virtual FVector calc_vel(double time, double x, double y, double z) = 0;
	virtual double calc_pres(double time, double x, double y, double z, FVector vel) = 0;
	TArray<FVector> CalculateLocations(FIntVector resolution, bool isApplyBias = true);
	TArray<FVector> CalculateFlatLocations(float xRes, float yRes, bool isApplyBias = true);
	FVector GetDistanceBetweenSensors(FIntVector resolution);

	const FVector LowerLimits = FVector(1, 1, 1);		// A
	const FVector UpperLimits = FVector(6, 6, 6);		// B

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