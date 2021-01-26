// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "Test1.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UTest1 : public UCalculator
{
	GENERATED_BODY()
public:
	UTest1();
	
	// Унаследовано через Calculator
	virtual FVector Calc_vel(double time, double x, double y, double z) override;
	virtual double Calc_pres(double time, double x, double y, double z, FVector vel) override;
};
