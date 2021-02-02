// Fill out your copyright notice in the Description page of Project Settings.


#include "Test6.h"

UTest6::UTest6()
{
}

FVector UTest6::Calc_vel(float time, FVector pos)
{
	FVector velocity = FVector();
	velocity.X = -pos.Y * pos.Z;
	velocity.Y = pos.X * pos.Z;	// ? В программе FlowTests почему-то sin(x).
	velocity.Z = 0;

	return velocity;
}

float UTest6::Calc_pres(float time, FVector pos)
{
	FVector vel = Calc_vel(time, pos);

	return -0.5 * FVector::DotProduct(vel, vel);
}