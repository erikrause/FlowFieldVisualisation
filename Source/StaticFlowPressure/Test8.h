#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "Test8.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UTest8 : public UCalculator
{
	GENERATED_BODY()
public:
	UTest8();

	virtual FVector Calc_vel(FVector position) const override;
	virtual float Calc_pres(FVector position) const override;
};