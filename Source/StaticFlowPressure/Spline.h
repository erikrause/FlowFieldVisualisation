// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Calculator.h"
#include "Spline.generated.h"

/**
 * 
 */
UCLASS()
class STATICFLOWPRESSURE_API USpline : public USplineComponent
{
	GENERATED_BODY()
	
public:

	static USpline* Construct(FVector splineStartPoint, UCalculator const* const* calculator);
	void Init(FVector splineStartPoint, UCalculator const* const* calculator);

	FVector SplineStartPoint;
	UCalculator const* const* Calculator;

	void UpdateSpline(int splinePointsLimit, int splineCalcStep);

	/// <summary>
	/// Частицы, принадлежащие этому сплайну.
	/// </summary>
	TArray<int> ParticleIds;
};
