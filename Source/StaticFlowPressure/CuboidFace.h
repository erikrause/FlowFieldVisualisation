// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class FaceAxis : uint8
{
	XY,
	XZ,
	YZ,
};

UENUM()
enum FacePosition
{
	Front,
	Back
};

/**
 * Грань куба.
 */
class STATICFLOWPRESSURE_API CuboidFace
{

public:

	CuboidFace(FVector cuboidStartPoint, FVector cuboidEndPoint, FaceAxis axis, FacePosition facePosition);
	~CuboidFace();

	FaceAxis Axis;
	FacePosition Position;
	FVector StartPoint;
	FVector EndPoint;
	FVector Bias;

	TArray<FVector> GetFacePointsGrid(float firstAxisRes, float secondAxisRes);
	FVector Get2DMask();
	FVector Get2DMaskNegative();
};
