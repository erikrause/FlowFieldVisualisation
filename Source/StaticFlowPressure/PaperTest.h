// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"

/**
 * 
 */
class STATICFLOWPRESSURE_API PaperTest : public Calculator
{
public:
	PaperTest();
	~PaperTest();

	// ������������ ����� Calculator
	virtual FVector calc_vel(double time, double x, double y, double z) override;
	virtual double calc_pres(double time, double x, double y, double z, FVector vel) override;
};
