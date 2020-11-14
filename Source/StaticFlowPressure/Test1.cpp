// Fill out your copyright notice in the Description page of Project Settings.

#include "Test1.h"
#include <map>

Test1::Test1()
{
}

Test1::~Test1()
{
}

double Test1::calc_vel(double time, double x, double y, double z, int coor)
{
    std::map<int, std::function<double()>> coorMap;
    coorMap[0] = [&]() { return exp(-epsilon * lyambda * time) * sin(sqrt(lyambda) * y); };
    coorMap[1] = [&]() { return exp(-epsilon * lyambda * time) * sin(sqrt(lyambda) * x); };
    
    return coorMap[coor]();
}

double Test1::calc_pres(double time, double x, double y, double z)
{
    /* DEBUG:
    double prob1 = exp(-2 * epsilon * time);
    double prob2 = (-0.5 * (pow(cos(x), 2) + pow(cos(y), 2)) + 0.25 * (cos(2 * x) + cos(2 * y)) - sin(x) * sin(y));
    double prob3 = -0.5 * (pow(cos(x), 2) + pow(cos(y), 2));
    double prob4 = 0.25 * (cos(2 * x) + cos(2 * y));
    double prob5 = sin(x) * sin(y);*/

    return exp(-2 * epsilon * time) * (-0.5 * (pow(cos(x), 2) + pow(cos(y), 2)) + 0.25 * (cos(2 * x) + cos(2 * y)) - sin(x) * sin(y));
}
