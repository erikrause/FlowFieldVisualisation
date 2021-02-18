// Fill out your copyright notice in the Description page of Project Settings.


#include "Test2.h"

UTest2::UTest2()
{
}

FVector UTest2::Calc_vel(FVector pos) const
{
    FVector vector = FVector();
    vector.X = exp(-Epsilon * Lyambda * Time) * sin(Lyambda * pos.Y);
    vector.Y = -exp(-Epsilon * Lyambda * Time) * sin(Lyambda * pos.X);
    vector.Z = 0;

    return vector;
}

float UTest2::Calc_pres(FVector pos) const
{
    return exp(-2 * Epsilon * Lyambda * Time) * cos(sqrt(Lyambda) * pos.X) * cos(sqrt(Lyambda) * pos.Y);
}