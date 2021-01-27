#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "Test7.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UTest7 : public UCalculator
{
	GENERATED_BODY()
public:
	UTest7();

	virtual FVector Calc_vel(float time, FVector position) override;
	virtual float Calc_pres(float time, FVector position) override;
};