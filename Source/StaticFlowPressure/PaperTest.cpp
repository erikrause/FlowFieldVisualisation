// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperTest.h"

UPaperTest::UPaperTest()
{
}

FVector UPaperTest::Calc_vel(double time, double x, double y, double z)
{
    FVector vector = FVector();

    float ex = exp(-Epsilon * time);
    vector.X = ex * (sin(y) - cos(z));
    vector.Y = ex * (sin(z) - cos(x));
    vector.Z = ex * (sin(x) - cos(y));

    return vector;
}

double UPaperTest::Calc_pres(double time, double x, double y, double z, FVector vel)
{
    return -FVector::DotProduct(vel, vel) / 2;
}
