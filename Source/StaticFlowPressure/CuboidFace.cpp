// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION


#include "CuboidFace.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

UCuboidFace::UCuboidFace()
{

#pragma region Set mesh asset

    static ConstructorHelpers::FObjectFinder<UStaticMesh> cubdoidSurfaceAsset(TEXT("StaticMesh'/Game/CuboidSurface/CuboidFaceMesh.CuboidFaceMesh'"));
    SetStaticMesh(cubdoidSurfaceAsset.Object);

    // Оптимизации
    SetCollisionProfileName(FName("OverlapAll"), false);
    SetCastShadow(false);
    SetLightAttachmentsAsGroup(true);
    SetRenderCustomDepth(true);

#pragma endregion

}

//UCuboidFace* UCuboidFace::Construct(FVector cuboidStartPoint, FVector cuboidEndPoint, FaceAxis axis, FacePosition facePosition)
//{
//    UCuboidFace* cuboidFace = NewObject<UCuboidFace>();
//    cuboidFace->Init(cuboidStartPoint, cuboidEndPoint, axis, facePosition);
//    return cuboidFace;
//}

void UCuboidFace::Init(FVector cuboidStartPoint, FVector cuboidEndPoint, FaceAxis axis, FacePosition facePosition)
{
    FVector pointMask;
    Axis = axis;
    Position = facePosition;

    if (Position == FacePosition::Front)
    {
        StartPoint = cuboidStartPoint;
        EndPoint = (cuboidEndPoint * GetAxisMask()) + (cuboidStartPoint * GetNormalVector());

        Bias = StartPoint * GetNormalVector();
    }
    else
    {
        StartPoint = cuboidEndPoint * GetNormalVector();
        EndPoint = cuboidEndPoint;

        Bias = EndPoint * GetNormalVector();
    }

    // CAUTION: this component needs to be attached first!
    SetRelativeLocation((((cuboidEndPoint + cuboidStartPoint) * GetAxisMask() / 2 + Bias)));  // *SizeMultipiler);	// Поставить ассет в центр грани.
    SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(FVector(0,0,0), GetNormalVector()));

    FVector scale = cuboidEndPoint - cuboidStartPoint;
    // Посколько face mesh - это плоскость, то нужно скейлить в только в YZ.
    if (Axis == FaceAxis::XY)
    {
        scale.Z = scale.X;
    }
    else if (Axis ==  FaceAxis::XZ)
    {
        scale.Z = scale.Y;
        scale.Y = scale.X;
    }

    SetRelativeScale3D(scale);
}

FVector UCuboidFace::GetAxisMask()
{
    FVector mask;

    if (Axis == FaceAxis::XY)
    {
        mask = FVector(1, 1, 0);
    }
    else if (Axis == FaceAxis::XZ)
    {
        mask=  FVector(1, 0, 1);
    }
    else if (Axis == FaceAxis::YZ)
    {
        mask = FVector(0, 1, 1);
    }

    return mask;
}

FVector UCuboidFace::GetNormalVector()
{
    FVector mask;

    if (Axis == FaceAxis::XY)
    {
        mask = FVector(0, 0, 1);
    }
    else if (Axis == FaceAxis::XZ)
    {
        mask = FVector(0, 1, 0);
    }
    else if (Axis == FaceAxis::YZ)
    {
        mask = FVector(1, 0, 0);
    }

    return mask;
}

TArray<FVector> UCuboidFace::GetPointsGrid(int firstAxisRes, int secondAxisRes, bool ischeckForActivated)    // TODO: FIntVector arg.
{
    if (IsActivated || !ischeckForActivated)
    {
        TArray<FVector> points = TArray<FVector>();

        float tempX, tempY;

        for (int i = 0; i <= firstAxisRes; i++)
        {
            tempX = i / (float)firstAxisRes;

            for (int j = 0; j <= secondAxisRes; j++)
            {
                tempY = j / (float)secondAxisRes;

                FVector point;
                if (Axis == FaceAxis::XY)
                {
                    point = FVector(tempX, tempY, 0);
                }
                else if (Axis == FaceAxis::XZ)
                {
                    point = FVector(tempX, 0, tempY);
                }
                else if (Axis == FaceAxis::YZ)
                {
                    point = FVector(0, tempX, tempY);
                }

                if (Position == FacePosition::Back)
                {
                    point = (StartPoint - EndPoint) * point + EndPoint;
                }
                else
                {
                    point = (EndPoint - StartPoint) * point + StartPoint;
                }
                points.Add(point);
            }
        }

        return points;
    }

    return TArray<FVector>();
}
