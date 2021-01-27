// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboidFace.h"

CuboidFace::CuboidFace(FVector cuboidStartPoint, FVector cuboidEndPoint, FaceAxis axis, FacePosition facePosition)
{
    FVector pointMask;
    Axis = axis;
    Position = facePosition;

    /*if (Axis == XY)
    {
        pointMask = FVector(1, 1, 0);       
    }
    else if (Axis == XZ)
    {
        pointMask = FVector(1, 0, 1);
    }
    else if (Axis == YZ)
    {
        pointMask = FVector(0, 1, 1);
    }

    // Получаем проекцию на плоскость по маске:
    StartPoint = cuboidStartPoint * pointMask;
    EndPoint = cuboidEndPoint * pointMask;
    */
    StartPoint = cuboidStartPoint;
    EndPoint = cuboidEndPoint;

    if (Position == Front)
    {
        Bias = StartPoint * Get2DMaskNegative();
    }
    else
    {
        Bias = EndPoint * Get2DMaskNegative();
    }

    // Лицевая или задняя грань:
    /*if (Position == Front)
    {
        Bias = 0;
    }
    else
    {
        FVector axisMask = FVector(1, 1, 1) - pointMask;
        FVector temp = axisMask * cuboidEndPoint;
        /Bias = temp.X + temp.Y + temp.Z;
    }*/
}

CuboidFace::~CuboidFace()
{
}

FVector CuboidFace::Get2DMask()
{
    if (Axis == FaceAxis::XY)
    {
        return FVector(1, 1, 0);
    }
    else if (Axis == FaceAxis::XZ)
    {
        return FVector(1, 0, 1);
    }
    else if (Axis == FaceAxis::YZ)
    {
        return FVector(0, 1, 1);
    }

    return FVector(0, 0, 0);
}

FVector CuboidFace::Get2DMaskNegative()
{
    if (Axis == FaceAxis::XY)
    {
        return FVector(0, 0, 1);
    }
    else if (Axis == FaceAxis::XZ)
    {
        return FVector(0, 1, 0);
    }
    else if (Axis == FaceAxis::YZ)
    {
        return FVector(1, 0, 0);
    }

    return FVector(0, 0, 0);
}

TArray<FVector> CuboidFace::GetFacePointsGrid(float firstAxisRes, float secondAxisRes)    // TODO: FIntVector arg.
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
            if (Axis == FaceAxis::XY)
            {
                location = FVector(tempX, tempY, 0);
            }
            else if (Axis == FaceAxis::XZ)
            {
                location = FVector(tempX, 0, tempY);
            }
            else if (Axis == FaceAxis::YZ)
            {
                location = FVector(0, tempX, tempY);
            }

            if (Position == Front)
            {
                location = (StartPoint - EndPoint) * location + EndPoint;
            }
            else
            {
                location = (EndPoint - StartPoint) * location + StartPoint;
            }
            locations.Add(location);
        }
    }

    return locations;
}
