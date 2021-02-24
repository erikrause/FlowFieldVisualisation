// Fill out your copyright notice in the Description page of Project Settings.


#include "Particle.h"

SplineParticle::SplineParticle(float distance)
{
	Distance = distance;
}

SplineParticle::~SplineParticle()
{
}

bool SplineParticle::operator==(SplineParticle const& right)
{
	return Distance == right.Distance;
}


//
//UParticle::~UParticle()
//{
//}
