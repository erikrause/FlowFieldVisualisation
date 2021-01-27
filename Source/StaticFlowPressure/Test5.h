#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "Test5.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UTest5 : public UCalculator
{
	GENERATED_BODY()
public:
	UTest5();

	virtual FVector Calc_vel(float time, FVector position) override;
	virtual float Calc_pres(float time, FVector position) override;
};