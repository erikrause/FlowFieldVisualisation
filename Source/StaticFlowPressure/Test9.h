#pragma once

#include "Calculator.h"
#include "CoreMinimal.h"
#include "Test9.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class STATICFLOWPRESSURE_API UTest9 : public UCalculator
{
	GENERATED_BODY()
public:
	UTest9();

	virtual FVector Calc_vel(float time, FVector position) override;
	virtual float Calc_pres(float time, FVector position) override;
};