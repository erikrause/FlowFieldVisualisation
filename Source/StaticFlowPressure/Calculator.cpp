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
#define radius 5.9//максимальный радиус сферы, нужен для задач со сферической симметрией
//#define A 1//начальная точка, для задач в прямоугольной области
//#define B 6//конечная точка, для задач в прямоугольной области
#define max_time 1//максимальное значение времени
//#define epsilon 1//параметр
#define sigma 1//параметр для теста 4
//#define lyambda 1//параметр для теста 2 и 4
#define nju 2//параметр для теста 4
//#define test_number 0//номер теста
#define components_number 8//количество компонет необходимо запомнить 8=3 комп. вектора скорости+поле давления+3 компоненты вектора магнитной индукции+поле концентрации
//#define map_5d(c,m,i,j,k)((c)*components_number*num_time_layers*num_p_along_x*num_p_along_y+(i)*num_time_layers*num_p_along_x*num_p_along_y+(j)*num_p_along_x*num_p_along_y+(k)*num_p_along_y+m)
double* massiv_data;
#define eps 1.e-4
#define M_PI 3.14159265358979323846

UCalculator::UCalculator()
{
}

double UCalculator::sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}
FVector UCalculator::Calc_vel(FVector pos) const
{
    return FVector();
}
float UCalculator::Calc_pres(FVector pos) const
{
    return 0.0;
}

FVector UCalculator::GetDistanceBetweenSensors(FIntVector resolution) const
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

void UCalculator::_initMaterial(UObject* parent)
{
    FString name;
    GetClass()->GetName(name);
    TArray<FStringFormatArg> args;
    args.Add(FStringFormatArg(name));
    args.Add(FStringFormatArg(name));
    static ConstructorHelpers::FObjectFinder<UMaterial> splineMaterialAsset(*FString::Format(TEXT("Material'/Game/SplineMaterials/SplineMaterial_{0}.SplineMaterial_{1}'"), args));
    if (splineMaterialAsset.Object != nullptr)
    {
        SplineMaterial = UMaterialInstanceDynamic::Create(splineMaterialAsset.Object, parent, FName(*FString::Format(TEXT("SplineMatreial_{0}"), args)));
    }
    else
    {
        if (name != FString("Base"))
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Не найден ассет для ") + name);
        }
    }
}
