// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Calculator.h"
#include "Components/SceneComponent.h"
#include "ParticleField.generated.h"


//class Particle
//{
//public:
//
//	//Particle(USpline* spline, UCalculator const* const* calculator)
//	//{
//	//	_spline = spline;
//	//	_calculator = calculator;
//	//}
//
//	float Distance = 0;
//	//float Velocity = 0;
//	float GetVelocity()
//	{
//		//FVector offset = _spline->GetRelative
//	}
//
//protected:
//
//	UCalculator const* const* _calculator;
//	USpline* _spline;
//
//};


UCLASS()
class STATICFLOWPRESSURE_API UParticleField : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UParticleField();


		
};
