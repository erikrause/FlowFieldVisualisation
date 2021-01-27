// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboidSurface.h"

CuboidSurface::CuboidSurface()
{

}
CuboidSurface::CuboidSurface(FVector startPoint, FVector endPoint) : CuboidSurface::CuboidSurface()
{
	StartPoint = startPoint;
	EndPoint = endPoint;

	Faces = TArray<CuboidFace>();

	for (int axis = 0; axis < 3; axis++)
	{
		Faces.Add(CuboidFace(StartPoint, EndPoint, (FaceAxis)axis, FacePosition::Front));
		Faces.Add(CuboidFace(StartPoint, EndPoint, (FaceAxis)axis, FacePosition::Back));
	}
}

CuboidSurface::~CuboidSurface()
{
}
