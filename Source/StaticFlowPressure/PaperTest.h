// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "PaperTest.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UPaperTest : public UCalculator
{
	GENERATED_BODY()
public:
	UPaperTest();

	// Унаследовано через Calculator
	FString Name = FString("PaperTest");
	virtual FVector Calc_vel(float time, FVector position) override;
	virtual float Calc_pres(float time, FVector position) override;
};
