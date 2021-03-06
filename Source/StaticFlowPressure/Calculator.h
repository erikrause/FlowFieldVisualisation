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

	// ����� �� ����� � �������� ������.
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
	virtual FVector Calc_vel(FVector position) const;// = 0;// PURE_VIRTUAL(UCalculator::calc_vel);
	virtual float Calc_pres(FVector position) const;// = 0; // PURE_VIRTUAL(UCalculator::calc_pres);
	//virtual float Calc_pres_by_vel(float time, FVector velocity);
	FVector GetDistanceBetweenSensors(FIntVector resolution) const;		// TODO: ����������� � Vector field.

	FVector LowerLimits = FVector(1, 1, 1);		// A
	FVector UpperLimits = FVector(6, 6, 6);		// B

	float Epsilon = 1;
	float Lyambda = 1;
	float Nju = 2;	//�������� ��� ����� 4
	float Time = 0;

	void _initMaterial(UObject* parent);
};
