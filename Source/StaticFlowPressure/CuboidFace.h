// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboidFace.generated.h"

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
UCLASS()
class STATICFLOWPRESSURE_API UCuboidFace : public UStaticMeshComponent	//USplinesStartPlane
{
	GENERATED_BODY()
public:

	UCuboidFace();

	//Parametrized constructor as: https://answers.unrealengine.com/questions/156055/passing-arguments-to-constructors-in-ue4.html
	//static UCuboidFace* Construct(FVector cuboidStartPoint, FVector cuboidEndPoint, FaceAxis axis, FacePosition facePosition);

	void Init(FVector cuboidStartPoint, FVector cuboidEndPoint, FaceAxis axis, FacePosition facePosition);

	FaceAxis Axis;
	FacePosition Position;
	FVector StartPoint;
	FVector EndPoint;
	FVector Bias;

	// if IsActive == true, to GetPointsGrid возвращает локации для сплайнов.
	TArray<FVector> GetPointsGrid(int firstAxisRes, int secondAxisRes, bool ischeckForActivated = true);
	FVector GetAxisMask();
	FVector GetNormalVector();

	UPROPERTY(EditAnywhere, Category = "Interaction")
		bool IsActivated = false;		// if IsActive == true, to GetPointsGrid возвращает локации для сплайнов.
};
