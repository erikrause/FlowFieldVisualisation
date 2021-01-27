// Fill out your copyright notice in the Description page of Project Settings.


#include "Test2.h"

UTest2::UTest2()
{
}

FVector UTest2::Calc_vel(float time, FVector pos)
{
    /*
    std::map<int, std::function<double()>> coorMap;
    coorMap[0] = [&]() { return exp(-epsilon * time) * cos(y); };
    coorMap[1] = [&]() { return -exp(-epsilon * time) * cos(x); };

    return coorMap[coor]();*/

    FVector vector = FVector();
    vector.X = exp(-Epsilon * time) * cos(pos.Y);
    vector.Y = -exp(-Epsilon * time) * cos(pos.X);
    vector.Z = 0;
    //vector.X = exp(-Epsilon * Lyambda * time) * cos(sqrt(Lyambda) * pos.Y);
    //vector.Y = -exp(-Epsilon * Lyambda * time) * cos(sqrt(Lyambda) * pos.X);
    //vector.Z = 0;

    return vector;
}

float UTest2::Calc_pres(float time, FVector pos)
{
    /* DEBUG:
    double prob1 = exp(-2 * epsilon * time);
    double prob2 = (-0.5 * (pow(cos(x), 2) + pow(cos(y), 2)) + 0.25 * (cos(2 * x) + cos(2 * y)) - sin(x) * sin(y));
    double prob3 = -0.5 * (pow(cos(x), 2) + pow(cos(y), 2));
    double prob4 = 0.25 * (cos(2 * x) + cos(2 * y));
    double prob5 = sin(x) * sin(y);*/

    //return exp(-2 * Epsilon * time) * (-0.5 * (pow(cos(pos.X), 2) + pow(cos(pos.Y), 2)) + 0.25 * (cos(2 * pos.X) + cos(2 * pos.Y)) - sin(pos.X) * sin(pos.Y));
    return exp(-2 * Epsilon * Lyambda * time) * cos(sqrt(Lyambda) * pos.X) * cos(sqrt(Lyambda) * pos.Y);
}