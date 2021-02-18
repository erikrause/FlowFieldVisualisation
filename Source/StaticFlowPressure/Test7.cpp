// Fill out your copyright notice in the Description page of Project Settings.


#include "Test7.h"

#define x pos.X
#define y pos.Y
#define z pos.Z

UTest7::UTest7()
{
}

FVector UTest7::Calc_vel(FVector pos) const
{
	float e1 = -Epsilon * Time;
	float e2 = -2 * Epsilon * Time;
	FVector velocity = FVector();
	velocity.X = (y - 3 * y * z - z) + e1 * (y * sin(z) - 2 * z * cos(y)) + e2 * (4 * y * sin(sqrt(2) * z) - z * sin(sqrt(2) * y));
	velocity.Y = -x + 3 * x * z + z + (-x * sin(z) + 3 * z * sin(x)) * exp(-Epsilon * Time) + (z * cos(sqrt(2) * x) - 4 * x * sin(sqrt(2) * z)) * exp(-2 * Epsilon * Time);
	velocity.Z = x - y + (2 * x * cos(y) - 3 * y * cos(x)) * exp(-Epsilon * Time) + (x * sin(sqrt(2) * y) - y * sin(sqrt(2) * x)) * exp(-2 * Epsilon * Time);

	return velocity;
}

float UTest7::Calc_pres(FVector pos) const
{
	FVector vel = Calc_vel(pos);

	return -0.5 * FVector::DotProduct(vel, vel);
}