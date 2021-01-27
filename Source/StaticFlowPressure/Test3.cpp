// Fill out your copyright notice in the Description page of Project Settings.


#include "Test3.h"

UTest3::UTest3()
{
}

FVector UTest3::Calc_vel(float time, FVector pos)
{
	FVector velocity = FVector();
	float e = exp(-Epsilon * time);
	velocity.X = -cos(pos.Z) + sin(pos.Y);
	velocity.Y = -cos(pos.X) + sin(pos.Z);
	velocity.Z = sin(pos.X) - cos(pos.Y);

	return e * velocity;
}

float UTest3::Calc_pres(float time, FVector pos)
{
	FVector vel = Calc_vel(time, pos);

	return -0.5 * FVector::DotProduct(vel, vel);
}
