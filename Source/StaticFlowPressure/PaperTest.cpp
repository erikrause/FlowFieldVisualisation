// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperTest.h"

UPaperTest::UPaperTest()
{
}

FVector UPaperTest::Calc_vel(float time, FVector pos)
{
    FVector vector = FVector();

    float ex = exp(-Epsilon * time);
    vector.X = ex * (sin(pos.Y) - cos(pos.Z));
    vector.Y = ex * (sin(pos.Z) - cos(pos.X));
    vector.Z = ex * (sin(pos.X) - cos(pos.Y));

    return vector;
}

float UPaperTest::Calc_pres(float time, FVector pos)
{
    return -FVector::DotProduct(Calc_vel(time, pos), Calc_vel(time, pos)) / 2;
}
