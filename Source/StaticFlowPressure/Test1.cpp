// Fill out your copyright notice in the Description page of Project Settings.

#include "Test1.h"

UTest1::UTest1()
{
}

FVector UTest1::Calc_vel(double time, double x, double y, double z)
{
    /*
    std::map<int, std::function<double()>> coorMap;
    coorMap[0] = [&]() { return exp(-epsilon * time) * cos(y); };
    coorMap[1] = [&]() { return -exp(-epsilon * time) * cos(x); };
    
    return coorMap[coor]();*/

    FVector vector = FVector();
    vector.X = exp(-Epsilon * time) * cos(y);
    vector.Y = -exp(-Epsilon * time) * cos(x);
    vector.Z = 0;

    return vector;
}

double UTest1::Calc_pres(double time, double x, double y, double z, FVector vel)
{
    /* DEBUG:
    double prob1 = exp(-2 * epsilon * time);
    double prob2 = (-0.5 * (pow(cos(x), 2) + pow(cos(y), 2)) + 0.25 * (cos(2 * x) + cos(2 * y)) - sin(x) * sin(y));
    double prob3 = -0.5 * (pow(cos(x), 2) + pow(cos(y), 2));
    double prob4 = 0.25 * (cos(2 * x) + cos(2 * y));
    double prob5 = sin(x) * sin(y);*/

    return exp(-2 * Epsilon * time) * (-0.5 * (pow(cos(x), 2) + pow(cos(y), 2)) + 0.25 * (cos(2 * x) + cos(2 * y)) - sin(x) * sin(y));
}
