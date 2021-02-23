// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Particle.h"
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

	static USpline* Construct(FVector startPoint, UCalculator const* const* calculator);
	void Init(FVector startPoint, UCalculator const* const* calculator);

	FVector StartPoint;
	UCalculator const* const* Calculator;

	void UpdateSpline(int curvePointsLimit, float curveStep);
	void SpawnParticle(float distance);
	void UpdateParticles(float deltaTime);

	/// <summary>
	/// Частицы, принадлежащие этому сплайну.
	/// </summary>
	//TArray<int> ParticleIds;
	TArray<SplineParticle*> Particles;
};
