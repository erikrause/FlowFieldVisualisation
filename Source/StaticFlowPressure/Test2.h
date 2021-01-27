// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "Test2.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UTest2 : public UCalculator
{
	GENERATED_BODY()
public:
	UTest2();

	virtual FVector Calc_vel(float time, FVector position) override;
	virtual float Calc_pres(float time, FVector position) override;
	//virtual float Calc_pres_by_vel(float time, FVector velocity) override;
};
