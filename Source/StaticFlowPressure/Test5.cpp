// Fill out your copyright notice in the Description page of Project Settings.


#include "Test5.h"

UTest5::UTest5()
{
}

FVector UTest5::Calc_vel(FVector pos) const
{

	FVector velocity = FVector();
	float e = exp(-Epsilon * Time);
	velocity.X = -pos.Z * sin(pos.Y);
	velocity.Y = pos.Z * sin(pos.Z);	// ? В программе FlowTests почему-то sin(x).
	velocity.Z = pos.X * sin(pos.Y) - pos.Y * cos(pos.X);

	return e * velocity;
}

float UTest5::Calc_pres(FVector pos) const
{
	FVector vel = Calc_vel(pos);

	return -0.5 * FVector::DotProduct(vel, vel);
}