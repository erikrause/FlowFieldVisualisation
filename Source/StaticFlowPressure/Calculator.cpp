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

/// elements of resolution must be > 1.
/// isApplyBias - применить смещнеие. ≈сли false - то точки будут в диапазоне [0, 1].
/// 
TArray<FVector> UCalculator::CalculateLocations(FIntVector resolution, bool isApplyBias) const
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

TArray<FVector> UCalculator::CalculateFlatLocations(float firstAxisRes, float secondAxisRes, FaceAxis plane, bool isOppositePlane, bool isApplyBias)    // TODO: FIntVector arg.
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
            if (plane == FaceAxis::XY)
            {
                location = FVector(tempX, tempY, 0);
            }
            else if (plane == FaceAxis::XZ)
            {
                location = FVector(tempX, 0, tempY);
            }
            else if (plane == FaceAxis::YZ)
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
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString("Ќе найден ассет дл€ ") + name);
        }
    }
}
