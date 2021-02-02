// Fill out your copyright notice in the Description page of Project Settings.


#include "Test2.h"

UTest2::UTest2()
{
}

FVector UTest2::Calc_vel(float time, FVector pos)
{
    FVector vector = FVector();
    vector.X = exp(-Epsilon * time) * cos(pos.Y);
    vector.Y = -exp(-Epsilon * time) * cos(pos.X);
    vector.Z = 0;

    return vector;
}

float UTest2::Calc_pres(float time, FVector pos)
{
    return exp(-2 * Epsilon * Lyambda * time) * cos(sqrt(Lyambda) * pos.X) * cos(sqrt(Lyambda) * pos.Y);
}