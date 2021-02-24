// Fill out your copyright notice in the Description page of Project Settings.

#include "PaperTest.h"

UPaperTest::UPaperTest()
{
    //_initMaterial(Name);
    int prob = 0;
}

FVector UPaperTest::Calc_vel(FVector pos) const
{
    FVector vector = FVector();

    float ex = exp(-Epsilon * Time);
    vector.X = ex * (sin(pos.Y) - cos(pos.Z));
    vector.Y = ex * (sin(pos.Z) - cos(pos.X));
    vector.Z = ex * (sin(pos.X) - cos(pos.Y));

    return vector;
}

float UPaperTest::Calc_pres(FVector pos) const
{
    FVector vel = Calc_vel(pos);
    return -FVector::DotProduct(vel, vel) / 2;
}
