#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "Test6.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UTest6 : public UCalculator
{
	GENERATED_BODY()
public:
	UTest6();

	virtual FVector Calc_vel(FVector position) const override;
	virtual float Calc_pres(FVector position) const override;
};