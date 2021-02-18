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
	virtual FVector Calc_vel(FVector position) const override;
	virtual float Calc_pres(FVector position) const override;
};