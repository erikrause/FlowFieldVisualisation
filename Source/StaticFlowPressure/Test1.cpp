// Fill out your copyright notice in the Description page of Project Settings.

#include "Test1.h"

UTest1::UTest1() : UCalculator()
{
}

FVector UTest1::Calc_vel(FVector pos) const
{

    FVector vector = FVector();
    vector.X = exp(-Epsilon * Time) * cos(pos.Y);
    vector.Y = -exp(-Epsilon * Time) * cos(pos.X);
    vector.Z = 0;

    return vector;
}

float UTest1::Calc_pres(FVector pos) const
{
    return exp(-2 * Epsilon * Time) * (-0.5 * (pow(cos(pos.X), 2) + pow(cos(pos.Y), 2)) + 0.25 * (cos(2 * pos.X) + cos(2 * pos.Y)) - sin(pos.X) * sin(pos.Y));
}
