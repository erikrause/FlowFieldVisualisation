// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "Spline.h"

USpline* USpline::Construct(FVector splineStartPoint, UCalculator const* const* calculator)
{
    USpline* spline = NewObject<USpline>();
    spline->Init(splineStartPoint, calculator);
    return spline;
}

void USpline::Init(FVector splineStartPoint, UCalculator const* const* calculator)
{
    SplineStartPoint = splineStartPoint;
	Calculator = calculator;
}

void USpline::UpdateSpline(int splinePointsLimit, int splineCalcStep)
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
	FVector newSplinePoint = (splinePoint + vel * splineCalcStep) - offset;
	//

	while (newSplinePoint.X + offset.X >= min.X && newSplinePoint.Y + offset.Y >= min.Y && newSplinePoint.Z + offset.Z >= min.Z &&
		newSplinePoint.X + offset.X <= max.X && newSplinePoint.Y + offset.Y <= max.Y && newSplinePoint.Z + offset.Z <= max.Z &&
		i < splinePointsLimit &&
		newSplinePoint != (GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Local)) &&	// ѕроверка на зацикливание кривой в пределах трех точек.
		isCorrectNormalized)	// ѕроверка на vel == 0 (например, когда t -> inf).
	{
		AddSplineLocalPoint(newSplinePoint);
		//splineComponent->SetSplinePointType(i + 1, ESplinePointType::Linear, true);		// TODO: check update arg.

		//SplineInstancedMesh->UpdateInstanceTransform()
		//FScopeLock(FCriticalSection())

		/*splineMeshComponent->SetStaticMesh(SplineMesh);
		splineMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		splineMeshComponent->SetRelativeLocation(newSplinePoint * SizeMultipiler);

		// Define the spline mesh points:
		FVector StartPoint = splineComponent->GetLocationAtSplinePoint(i - 1, ESplineCoordinateSpace::Type::Local);
		FVector StartTangent = splineComponent->GetTangentAtSplinePoint(i - 1, ESplineCoordinateSpace::Type::Local);
		FVector EndPoint = splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
		FVector EndTangent = splineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
		splineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
		*/

		// Next loop prepare:
		vel = (*Calculator)->Calc_vel(newSplinePoint + offset);
		isCorrectNormalized = vel.Normalize();
		newSplinePoint = (newSplinePoint + vel * splineCalcStep);
		i++;
	}
}
