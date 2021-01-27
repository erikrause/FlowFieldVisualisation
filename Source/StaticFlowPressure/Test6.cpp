// Fill out your copyright notice in the Description page of Project Settings.


#include "Test6.h"

UTest6::UTest6()
{
}

FVector UTest6::Calc_vel(float time, FVector pos)
{
	/*
	case 0: return exp(-epsilon * time) * (-z) * sin(y);
	case 1: return exp(-epsilon * time) * z * sin(x);
	case 2: return exp(-epsilon * time) * (x * sin(y) - y * sin(x));
	*/
	FVector velocity = FVector();
	velocity.X = -pos.Y * pos.Z;
	velocity.Y = pos.X * pos.Z;	// ? В программе FlowTests почему-то sin(x).
	velocity.Z = 0;

	return velocity;
}

float UTest6::Calc_pres(float time, FVector pos)
{
	//return exp(2 * Epsilon * time * (pow(Nju, 2) - pow(Lyambda, 2))) * cos(Lyambda * pos.X) * cos(Lyambda * pos.Y);
	FVector vel = Calc_vel(time, pos);

	return -0.5 * FVector::DotProduct(vel, vel);
}