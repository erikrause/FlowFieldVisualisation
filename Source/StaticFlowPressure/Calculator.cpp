// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "Calculator.h"
#include <vector>
#include <map>
#include <cmath>
/*
#define num_p_along_x 40
#define num_p_along_y 40
#define num_p_along_z 40
*/
#define num_time_layers 40
#define radius 5.9//максимальный радиус сферы, нужен дл€ задач со сферической симметрией
//#define A 1//начальна€ точка, дл€ задач в пр€моугольной области
//#define B 6//конечна€ точка, дл€ задач в пр€моугольной области
#define max_time 1//максимальное значение времени
//#define epsilon 1//параметр
#define sigma 1//параметр дл€ теста 4
//#define lyambda 1//параметр дл€ теста 2 и 4
#define nju 2//параметр дл€ теста 4
//#define test_number 0//номер теста
#define components_number 8//количество компонет необходимо запомнить 8=3 комп. вектора скорости+поле давлени€+3 компоненты вектора магнитной индукции+поле концентрации
//#define map_5d(c,m,i,j,k)((c)*components_number*num_time_layers*num_p_along_x*num_p_along_y+(i)*num_time_layers*num_p_along_x*num_p_along_y+(j)*num_p_along_x*num_p_along_y+(k)*num_p_along_y+m)
double* massiv_data;
#define eps 1.e-4
#define M_PI 3.14159265358979323846

Calculator::Calculator()
{
    epsilon = 1;
    lyambda = 1;
}

Calculator::~Calculator()
{
}

double Calculator::sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}
/*
double Calculation::calc_vel(double time, double x, double y, double z, int coor)//поле скорости coor={0,1,2} - номер компоненты вектора скорости
{
    switch (test_number)
    {
    case 0:
        switch (coor)
        {
        case 0: return exp(-epsilon * time) * cos(y);
        case 1: return -exp(-epsilon * time) * cos(x);
        default:return 0;
        }
    case 1:
        switch (coor)
        {
        case 0: return exp(-epsilon * lyambda * time) * sin(sqrt(lyambda) * y);
        case 1: return exp(-epsilon * lyambda * time) * sin(sqrt(lyambda) * x);
        default:return 0;
        }
    case 2:
        switch (coor)
        {
        case 0: return exp(-epsilon * time) * (-cos(z) + sin(y));
        case 1: return exp(-epsilon * time) * (-cos(x) + sin(z));
        case 2: return exp(-epsilon * time) * (-cos(y) + sin(x));
        default:return 0;
        }
    case 3:
        switch (coor)
        {
        case 0: return 2 * exp(-epsilon * time) * sin(x - M_PI / 4) * cos(z - y + M_PI / 4);
        case 1: return 2 * exp(-epsilon * time) * sin(y - M_PI / 4) * cos(x - z + M_PI / 4);
        case 2: return 2 * exp(-epsilon * time) * sin(z - M_PI / 4) * cos(y - x + M_PI / 4);
        default:return 0;
        }
    case 4:
        switch (coor)
        {
        case 0: return exp(epsilon * time * (pow(nju, 2) - pow(lyambda, 2))) * exp(nju * z) * sin(lyambda * y);
        case 1: return exp(epsilon * time * (pow(nju, 2) - pow(lyambda, 2))) * exp(-nju * z) * sin(lyambda * x);
        case 2: return 0;
        default:return 0;
        }
    case 5:
        switch (coor)
        {
        case 0: return exp(-epsilon * time) * (-z) * sin(y);
        case 1: return exp(-epsilon * time) * z * sin(x);
        case 2: return exp(-epsilon * time) * (x * sin(y) - y * sin(x));
        default:return 0;
        }
    case 6:
        switch (coor)
        {
        case 0: return -3 * y * z;
        case 1: return 3 * x * z;
        case 2: return 0;
        default:return 0;
        }
    case 7:
        switch (coor)
        {
        case 0: return y - 3 * y * z - z + (y * sin(z) - 2 * z * cos(y)) * exp(-epsilon * time) + (4 * y * sin(sqrt(2) * z) - z * sin(sqrt(2) * y)) * exp(-2 * epsilon * time);
        case 1: return -x + 3 * x * z + z + (-x * sin(z) + 3 * z * sin(x)) * exp(-epsilon * time) + (z * cos(sqrt(2) * x) - 4 * x * sin(sqrt(2) * z)) * exp(-2 * epsilon * time);;
        case 2: return x - y + (2 * x * cos(y) - 3 * y * cos(x)) * exp(-epsilon * time) + (x * sin(sqrt(2) * y) - y * sin(sqrt(2) * x)) * exp(-2 * epsilon * time);
        default:return 0;
        }
    case 8:
        switch (coor)
        {
        case 0: return y - z;
        case 1: return -x + z;;
        case 2: return x - y;
        default:return 0;
        }
    case 9:
        switch (coor)
        {
        case 0: return -y * z;
        case 1: return x * z;;
        case 2: return 0;
        default:return 0;
        }
    default:return 0;
    }
}

double Calculation::calc_pres(double time, double x, double y, double z)//поле давлени€
{
    switch (test_number)
    {
    case 0:
        return exp(-2 * epsilon * time) * (-0.5 * (pow(cos(x), 2) + pow(cos(y), 2)) + 0.25 * (cos(2 * x) + cos(2 * y)) - sin(x) * sin(y));
    case 1:
        return exp(-2 * epsilon * lyambda * time) * cos(sqrt(lyambda) * x) * cos(sqrt(lyambda) * y);
    case 2:
        return -0.5 * exp(-2 * epsilon * time) * (pow(-cos(z) + sin(y), 2) + pow(-cos(x) + sin(z), 2) + pow(-cos(y) + sin(x), 2));
    case 3:
        return -2 * exp(-2 * epsilon * time) * (pow(sin(x - M_PI / 4) * cos(z - y + M_PI / 4), 2) + pow(sin(y - M_PI / 4) * cos(x - z + M_PI / 4), 2) + pow(sin(z - M_PI / 4) * cos(y - x + M_PI / 4), 2));
    case 4:
        return exp(2 * epsilon * time * (pow(nju, 2) - pow(lyambda, 2))) * cos(lyambda * x) * cos(lyambda * y);
    case 5:
        return -0.5 * (pow(calc_vel(time, x, y, z, 0), 2) + pow(calc_vel(time, x, y, z, 1), 2) + pow(calc_vel(time, x, y, z, 2), 2));
    case 6:
        return -0.5 * (pow(calc_vel(time, x, y, z, 0), 2) + pow(calc_vel(time, x, y, z, 1), 2) + pow(calc_vel(time, x, y, z, 2), 2));
    case 7:
        return -0.5 * (pow(calc_vel(time, x, y, z, 0), 2) + pow(calc_vel(time, x, y, z, 1), 2) + pow(calc_vel(time, x, y, z, 2), 2));
    case 8:
        return -0.5 * (pow(calc_vel(time, x, y, z, 0), 2) + pow(calc_vel(time, x, y, z, 1), 2) + pow(calc_vel(time, x, y, z, 2), 2));
    case 9:
        return -0.5 * (pow(calc_vel(time, x, y, z, 0), 2) + pow(calc_vel(time, x, y, z, 1), 2) + pow(calc_vel(time, x, y, z, 2), 2));
    default: return 0;
    }
}*/

/// elements of resolution must be > 1.
/// isApplyBias - применить смещнеие. ≈сли false - то точки будут в диапазоне [0, 1].
/// 
TArray<FVector> Calculator::CalculateLocations(FIntVector resolution, bool isApplyBias)
{
    // ѕроверка, если одна из осей <= 1 (костыль, TODO):
    FVector axisMask = FVector(1, 1, 1);
    
    for (int axis = 0; axis < 3; axis++)
    {
        if (resolution[axis] <= 1)
        {
            resolution[axis] = 2;    // минимальное значение, возможное в цикле.
            axisMask[axis] = 0;     // ось, котора€ <=, обнулитс€
        }
    }

    double tempX, tempY, tempZ;

    TArray<FVector> locations = TArray<FVector>();

    for (int i = 0; i < resolution.X; i++)
    {
        tempX = (double)i / (double)(resolution.X - 1);

        for (int j = 0; j < resolution.Y; j++)
        {
            tempY = (double)j / (double)(resolution.Y - 1);

            for (int k = 0; k < resolution.Z; k++)
            {
                tempZ = (double)k / (double)(resolution.Z - 1);

                FVector location = FVector(tempX, tempY, tempZ);
                location *= axisMask;

                if (isApplyBias)
                {
                    location = (UpperLimits - LowerLimits)* location + LowerLimits;
                }

                locations.Add(location);
            }
        }
    }

    return locations;
}

TArray<FVector> Calculator::CalculateFlatLocations(float firstAxisRes, float secondAxisRes, TestPlane plane, bool isOppositePlane, bool isApplyBias)    // TODO: FIntVector arg.
{
    TArray<FVector> locations = TArray<FVector>();

    double tempX, tempY;

    for (int i = 0; i < firstAxisRes; i++)
    {
        tempX = (double)i / (double)(firstAxisRes - 1);

        for (int j = 0; j < secondAxisRes; j++)
        {
            tempY = (double)j / (double)(secondAxisRes - 1);

            FVector location;
            if (plane == TestPlane::XY)
            {
                location = FVector(tempX, tempY, 0);
            }
            else if (plane == TestPlane::XZ)
            {
                location = FVector(tempX, 0, tempY);
            }
            else if (plane == TestPlane::YZ)
            {
                location = FVector(0, tempX, tempY);
            }

            if (isApplyBias)
            {
                if (isOppositePlane) 
                {
                    location = (LowerLimits - UpperLimits) * location + UpperLimits;
                }
                else
                {
                    location = (UpperLimits - LowerLimits) * location + LowerLimits;
                }
                
            }
            locations.Add(location);
        }
    }

    return locations;
}

FVector Calculator::GetDistanceBetweenSensors(FIntVector resolution)
{
    /*int num_p_along_x = scale;
    return ((B - A) * (double)2 / (double)(num_p_along_x - 1) + A) -
        ((B - A) * (double)1 / (double)(num_p_along_x - 1) + A);*/

    FVector distance = FVector();

    distance.X = ((UpperLimits.X - LowerLimits.X) * (double)2 / (double)(resolution.X - 1) + LowerLimits.X) -
        ((UpperLimits.X - LowerLimits.X) * (double)1 / (double)(resolution.X - 1) + LowerLimits.X);

    distance.Y = ((UpperLimits.Y - LowerLimits.Y) * (double)2 / (double)(resolution.Y - 1) + LowerLimits.Y) -
        ((UpperLimits.Y - LowerLimits.Y) * (double)1 / (double)(resolution.Y - 1) + LowerLimits.Y);

    distance.Z = ((UpperLimits.Z - LowerLimits.Z) * (double)2 / (double)(resolution.Z - 1) + LowerLimits.Z) -
        ((UpperLimits.Z - LowerLimits.Z) * (double)1 / (double)(resolution.Z - 1) + LowerLimits.Z);

    return distance;
}
