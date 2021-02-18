// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboidSurface.h"

//UCuboidSurface* UCuboidSurface::Construct(FVector startPoint, FVector endPoint)
//{
//	UCuboidSurface* cuboidSurface = NewObject<UCuboidSurface>();
//	cuboidSurface->Init(startPoint, endPoint);
//	return cuboidSurface;
//}

UCuboidSurface::UCuboidSurface()
{
	for (int faceId = 0; faceId < 6; faceId++)
	{
		UCuboidFace* face = CreateDefaultSubobject<UCuboidFace>(*(FString("Face_") + FString::FromInt(faceId)));
		face->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		Faces.Add(face);
	}
}

void UCuboidSurface::Init(FVector startPoint, FVector endPoint)
{
	StartPoint = startPoint;
	EndPoint = endPoint;

	// Init front faces positions:
	for (int faceId = 0; faceId < 3; faceId++)
	{
		UCuboidFace* frontFace = Faces[faceId];
		frontFace->Init(StartPoint, EndPoint, (FaceAxis)faceId, FacePosition::Front);
	}

	// Init back faces positions:
	for (int faceId = 3; faceId < 6; faceId++)
	{
		UCuboidFace* backFace = Faces[faceId];
		backFace->Init(StartPoint, EndPoint, (FaceAxis)(faceId - 3), FacePosition::Back);
	}
}

UCuboidFace* UCuboidSurface::GetFaceBy(FaceAxis faceAxis, FacePosition facePosition)
{
	UCuboidFace** face = Faces.FindByPredicate([faceAxis, facePosition](const UCuboidFace* face)
		{
			return (face->Axis == faceAxis) && (face->Position == facePosition);
		});

	return *face;
}
