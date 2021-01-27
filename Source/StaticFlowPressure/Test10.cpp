#include "Test10.h"

#define x pos.X
#define y pos.Y
#define z pos.Z

UTest10::UTest10()
{
}

FVector UTest10::Calc_vel(float time, FVector pos)
{
	float e = -Epsilon * time;
	FVector velocity = FVector();
	velocity.X = sin(y) - cos(z);
	velocity.Y = sin(z) - cos(x);
	velocity.Z = sin(x) - cos(y);

	return e * velocity;
}

float UTest10::Calc_pres(float time, FVector pos)
{
	//return exp(2 * Epsilon * time * (pow(Nju, 2) - pow(Lyambda, 2))) * cos(Lyambda * pos.X) * cos(Lyambda * pos.Y);
	FVector vel = Calc_vel(time, pos);

	return -0.5 * FVector::DotProduct(vel, vel);
}