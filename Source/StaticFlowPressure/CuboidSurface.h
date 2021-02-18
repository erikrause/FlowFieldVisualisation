// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboidFace.h"
#include "CuboidSurface.generated.h"

/**
 * Поверхность куба. 
 */
UCLASS()
class STATICFLOWPRESSURE_API UCuboidSurface : public USceneComponent
{
	GENERATED_BODY()
public:
	UCuboidSurface();
	//static UCuboidSurface* Construct(FVector startPoint, FVector endPoint);
	void Init(FVector startPoint, FVector endPoint);

	UCuboidFace* GetFaceBy(FaceAxis faceAxis, FacePosition facePosition);

	//TArray<FVector> GetSurfacePointGrid(FVector resolution);

	FVector StartPoint;
	FVector EndPoint;
	UPROPERTY(EditAnywhere, Category = "Visualisation")
		TArray<UCuboidFace*> Faces;
};
