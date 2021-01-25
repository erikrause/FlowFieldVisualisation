// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "PaperTest.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UPaperTest : public UCalculator
{
	GENERATED_BODY()
public:
	UPaperTest();
	~UPaperTest();

	// ������������ ����� Calculator
	virtual FVector calc_vel(double time, double x, double y, double z) override;
	virtual double calc_pres(double time, double x, double y, double z, FVector vel) override;
};
