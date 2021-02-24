// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Calculator.h"
//#include "opele.generated.h"

/**
 * �������, ������������� �������.
 */
class STATICFLOWPRESSURE_API SplineParticle
{

public:

	SplineParticle(float distance);
	~SplineParticle();

	float Distance;

	bool operator ==(SplineParticle const& right);	// ����� ��� ������ � �������.

};
