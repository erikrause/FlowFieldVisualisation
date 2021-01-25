// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboidFace.h"

/**
 * Поверхность куба. 
 */
class STATICFLOWPRESSURE_API CuboidSurface
{
public:
	CuboidSurface();
	CuboidSurface(FVector startPoint, FVector endPoint);
	~CuboidSurface();

	//TArray<FVector> GetSurfacePointGrid(FVector resolution);

	FVector StartPoint;
	FVector EndPoint;
	TArray<CuboidFace> Faces;
};
