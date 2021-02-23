// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "Spline.h"

USpline* USpline::Construct(FVector startPoint, UCalculator const* const* calculator)
{
    USpline* spline = NewObject<USpline>();
    spline->Init(startPoint, calculator);
    return spline;
}

void USpline::Init(FVector startPoint, UCalculator const* const* calculator)
{
    StartPoint = startPoint;
	Calculator = calculator;
}

void USpline::UpdateSpline(int curvePointsLimit, float curveStep)
{
	int i;
	//if (!isContinue)
	//{
	ClearSplinePoints();
	AddSplineLocalPoint(FVector(0, 0, 0));		// Init point/
	i = 0;
	//else
	//{
	//	i = GetNumberOfSplinePoints() - 1;
	//	for (i; i >= splinePointsLimit; i--)
	//	{
	//		splineComponent->RemoveSplinePoint(i, false);
	//}

	FVector splineComponentLocation = GetRelativeLocation();
	FVector min = (*Calculator)->LowerLimits;
	FVector max = (*Calculator)->UpperLimits;

	// Loop init:
	FVector offset = splineComponentLocation;	// / SizeMultipiler;
	FVector splinePoint = GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local) + offset;

	FVector vel = (*Calculator)->Calc_vel(splinePoint);
	bool isCorrectNormalized = vel.Normalize();
	FVector newSplinePoint = (splinePoint + vel * curveStep) - offset;
	//

	while (newSplinePoint.X + offset.X >= min.X && newSplinePoint.Y + offset.Y >= min.Y && newSplinePoint.Z + offset.Z >= min.Z &&
	newSplinePoint.X + offset.X <= max.X && newSplinePoint.Y + offset.Y <= max.Y && newSplinePoint.Z + offset.Z <= max.Z &&
	i < curvePointsLimit &&
	newSplinePoint != (GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local)) &&	// �������� �� ������������ ������ � �������� ���� �����.
	isCorrectNormalized)	// �������� �� vel == 0 (��������, ����� t -> inf).
	{
		AddSplineLocalPoint(newSplinePoint);
		//splineComponent->SetSplinePointType(i + 1, ESplinePointType::Linear, true);		// TODO: check update arg.

		//FScopeLock(FCriticalSection())

		// Next loop prepare:
		vel = (*Calculator)->Calc_vel(newSplinePoint + offset);
		isCorrectNormalized = vel.Normalize();
		newSplinePoint = (newSplinePoint + vel * curveStep);
		i++;
	}
}

void USpline::SpawnParticle(float distance)
{
	//FVector location = GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::Local);
	Particles.Add(new SplineParticle(distance));
}

// ��������� ������ ������ �� ������� � ������� �������, �������� �� ������� �������.
void USpline::UpdateParticles(float deltaTime)
{
	TArray<SplineParticle*> outsideParticles;		// �������, �������� �� ������� �������.

	// TODO: �������������� ����.
	for (SplineParticle* particle : Particles)
	{
		FVector offset = GetRelativeLocation();
		FVector particlePosition = GetLocationAtDistanceAlongSpline(particle->Distance, ESplineCoordinateSpace::Local) + offset;
		FVector particleVelocity = (*Calculator)->Calc_vel(particlePosition);
		particle->Distance += particleVelocity.Size() * deltaTime;	// TODO: ��������� ��������, ����������� Size().

		if ((particle->Distance > GetSplineLength()) || (particle->Distance < 0))
		{
			outsideParticles.Add(particle);
		}
	}

	// Remove outside particles:
	for (SplineParticle* particle : outsideParticles)
	{
		//Particles.FilterByPredicate([](return particle.De))
		Particles.Remove(particle);
	}
}
