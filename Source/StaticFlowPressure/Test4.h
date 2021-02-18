#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "Test4.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UTest4 : public UCalculator
{
	GENERATED_BODY()
public:
	UTest4();

	virtual FVector Calc_vel(FVector position) const override;
	virtual float Calc_pres(FVector position) const override;
};