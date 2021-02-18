#include "Test10.h"

#define x pos.X
#define y pos.Y
#define z pos.Z

UTest10::UTest10()
{
}

FVector UTest10::Calc_vel(FVector pos) const
{
	float e = -Epsilon * Time;
	FVector velocity = FVector();
	velocity.X = sin(y) - cos(z);
	velocity.Y = sin(z) - cos(x);
	velocity.Z = sin(x) - cos(y);

	return e * velocity;
}

float UTest10::Calc_pres(FVector pos) const
{
	FVector vel = Calc_vel(pos);

	return -0.5 * FVector::DotProduct(vel, vel);
}