// Fill out your copyright notice in the Description page of Project Settings.


#include "Test4.h"

UTest4::UTest4()
{
}

FVector UTest4::Calc_vel(FVector pos) const
{
	FVector velocity = FVector();
	float e = exp((Nju * Nju - Lyambda * Lyambda) * Epsilon * Time);
	velocity.X = exp(Nju * pos.Z) * sin(Lyambda * pos.Y);
	velocity.Y = exp(-Nju * pos.Z) * sin(Lyambda * pos.X);
	velocity.Z = 0;

	return e * velocity;
}

float UTest4::Calc_pres(FVector pos) const
{
	return exp(2 * Epsilon * Time * (pow(Nju, 2) - pow(Lyambda, 2))) * cos(Lyambda * pos.X) * cos(Lyambda * pos.Y);
}