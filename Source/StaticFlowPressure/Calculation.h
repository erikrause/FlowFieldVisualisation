// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class STATICFLOWPRESSURE_API Calculator
{
public:
	Calculator();
	~Calculator();
	static double sigmoid(double x);
	virtual double calc_vel(double time, double x, double y, double z, int coor) = 0;
	virtual double calc_pres(double time, double x, double y, double z) = 0;
	static TArray<FVector*>* CalculateLocations(int scale);
	static double GetDistanceBetweenSensors(int scale);

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