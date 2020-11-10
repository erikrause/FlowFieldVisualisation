// Fill out your copyright notice in the Description page of Project Settings.
//PRAGMA_DISABLE_OPTIMIZATION

#include "Calculation.h"
#include <vector>
#include <map>
#include <cmath>
/*
#define num_p_along_x 40
#define num_p_along_y 40
#define num_p_along_z 40
*/
#define num_time_layers 40
#define radius 5.9//������������ ������ �����, ����� ��� ����� �� ����������� ����������
#define A 1//��������� �����, ��� ����� � ������������� �������
#define B 6//�������� �����, ��� ����� � ������������� �������
#define max_time 1//������������ �������� �������
//#define epsilon 1//��������
#define sigma 1//�������� ��� ����� 4
//#define lyambda 1//�������� ��� ����� 2 � 4
#define nju 2//�������� ��� ����� 4
//#define test_number 0//����� �����
#define components_number 8//���������� �������� ���������� ��������� 8=3 ����. ������� ��������+���� ��������+3 ���������� ������� ��������� ��������+���� ������������
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
double Calculation::calc_vel(double time, double x, double y, double z, int coor)//���� �������� coor={0,1,2} - ����� ���������� ������� ��������
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

double Calculation::calc_pres(double time, double x, double y, double z)//���� ��������
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

TArray<FVector*>* Calculator::CalculateLocations(int scale)
{
    TArray<FVector*>* locations = new TArray<FVector*>();

    double temp_x, temp_y, temp_z;
    int num_p_along_x = scale;
    int num_p_along_y = scale;
    int num_p_along_z = scale;

    for (int i = 0; i < num_p_along_x; i++)
    {
        temp_x = (B - A) * (double)i / (double)(num_p_along_x - 1) + A;
        for (int j = 0; j < num_p_along_y; j++)
        {
            temp_y = (B - A) * (double)j / (double)(num_p_along_y - 1) + A;
            for (int k = 0; k < num_p_along_z; k++)
            {
                temp_z = (B - A) * (double)k / (double)(num_p_along_z - 1) + A;

                //locations->push_back(loc);
                locations->Add(new FVector(temp_x, temp_y, temp_z));
            }
        }
    }
    return locations;
}

double Calculator::GetDistanceBetweenSensors(int scale)
{
    int num_p_along_x = scale;
    return ((B - A) * (double)2 / (double)(num_p_along_x - 1) + A) -
        ((B - A) * (double)1 / (double)(num_p_along_x - 1) + A);
}