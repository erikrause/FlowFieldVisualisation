// Fill out your copyright notice in the Description page of Project Settings.


#include "Test5.h"

UTest5::UTest5()
{
}

FVector UTest5::Calc_vel(float time, FVector pos)
{
	/*
	case 0: return exp(-epsilon * time) * (-z) * sin(y);
	case 1: return exp(-epsilon * time) * z * sin(x);
	case 2: return exp(-epsilon * time) * (x * sin(y) - y * sin(x));
	*/

	FVector velocity = FVector();
	float e = exp(-Epsilon * time);
	velocity.X = -pos.Z * sin(pos.Y);
	velocity.Y = pos.Z * sin(pos.Z);	// ? В программе FlowTests почему-то sin(x).
	velocity.Z = pos.X * sin(pos.Y) - pos.Y * cos(pos.X);

	return e * velocity;
}

float UTest5::Calc_pres(float time, FVector pos)
{
	//return exp(2 * Epsilon * time * (pow(Nju, 2) - pow(Lyambda, 2))) * cos(Lyambda * pos.X) * cos(Lyambda * pos.Y);
	FVector vel = Calc_vel(time, pos);

	return -0.5 * FVector::DotProduct(vel, vel);
}