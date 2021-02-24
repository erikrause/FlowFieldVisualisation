#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "Test3.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UTest3 : public UCalculator
{
	GENERATED_BODY()
public:
	UTest3();

	virtual FVector Calc_vel(FVector position) const override;
	virtual float Calc_pres(FVector position) const override;
};
