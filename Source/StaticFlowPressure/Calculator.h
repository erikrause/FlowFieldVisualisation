// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboidFace.h"
#include "Calculator.generated.h"
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
/*
UENUM()
enum class TestPlane : uint8
{
	XY,
	XZ,
	YZ
};*/

UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UCalculator : public UObject
{
	GENERATED_BODY()
public:

	UCalculator();
	virtual ~UCalculator() = default;
	FString Name; //= FString("Base");
	UMaterialInstanceDynamic* SplineMaterial;

	static double sigmoid(double x);
	virtual FVector Calc_vel(float time, FVector position);// = 0;// PURE_VIRTUAL(UCalculator::calc_vel);
	virtual float Calc_pres(float time, FVector position);// = 0; // PURE_VIRTUAL(UCalculator::calc_pres);
	//virtual float Calc_pres_by_vel(float time, FVector velocity);
	TArray<FVector> CalculateLocations(FIntVector resolution, bool isApplyBias = true);
	TArray<FVector> CalculateFlatLocations(float firstAxisRes, float secondAxisRes, FaceAxis plane, bool isOppositePlane, bool isApplyBias = true);
	FVector GetDistanceBetweenSensors(FIntVector resolution);

	const FVector LowerLimits = FVector(1, 1, 1);		// A
	const FVector UpperLimits = FVector(6, 6, 6);		// B

	float Epsilon = 1;
	float Lyambda = 1;
	float Nju = 2;	//параметр для теста 4

	void _initMaterial(UObject* parent);
};
