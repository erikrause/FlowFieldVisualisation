// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboidFace.h"
#include "SplinesStartArea.h"
#include "CuboidSurface.generated.h"

/**
 * ѕоверхность куба. 
 */
UCLASS()
class STATICFLOWPRESSURE_API UCuboidSurface : public USceneComponent, public ISplinesStartArea
{
	GENERATED_BODY()
public:
	UCuboidSurface();
	//static UCuboidSurface* Construct(FVector startPoint, FVector endPoint);
	void Init(FVector startPoint, FVector endPoint);

	void UpdateSurface(FVector startPoint, FVector endPoint);

	UCuboidFace* GetFaceBy(FaceAxis faceAxis, FacePosition facePosition);

	bool IsCheckFaceForActivated = true;		// ѕровер€ет, что грань активирована, перед тем как вернуть SplineStartsLocations.
	virtual TArray<FVector> GetSplinesStartLocations(FIntVector resolution) override;

	FVector StartPoint;
	FVector EndPoint;
	UPROPERTY(EditAnywhere, Category = "Visualisation")
		TArray<UCuboidFace*> Faces;
};
