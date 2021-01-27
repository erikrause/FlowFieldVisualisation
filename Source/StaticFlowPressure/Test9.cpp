#include "Test9.h"

#define x pos.X
#define y pos.Y
#define z pos.Z

UTest9::UTest9()
{
}

FVector UTest9::Calc_vel(float time, FVector pos)
{
	FVector velocity = FVector();
	velocity.X = -y * z;
	velocity.Y = x * z;
	velocity.Z = 0;

	return velocity;
}

float UTest9::Calc_pres(float time, FVector pos)
{
	//return exp(2 * Epsilon * time * (pow(Nju, 2) - pow(Lyambda, 2))) * cos(Lyambda * pos.X) * cos(Lyambda * pos.Y);
	FVector vel = Calc_vel(time, pos);

	return -0.5 * FVector::DotProduct(vel, vel);
}