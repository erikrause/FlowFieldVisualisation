// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Calculation.h"
#include "CoreMinimal.h"

/**
 * 
 */
class STATICFLOWPRESSURE_API Test1 : public Calculator
{
public:
	Test1();
	~Test1();
	
	// Унаследовано через Calculator
	virtual double calc_vel(double time, double x, double y, double z, int coor) override;
	virtual double calc_pres(double time, double x, double y, double z) override;
};
