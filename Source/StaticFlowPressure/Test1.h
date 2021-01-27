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
	
	// ������������ ����� Calculator
	virtual FVector Calc_vel(float time, FVector position) override;
	virtual float Calc_pres(float time, FVector position) override; 
};