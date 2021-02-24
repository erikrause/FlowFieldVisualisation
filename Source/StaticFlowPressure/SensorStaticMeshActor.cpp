// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "SensorStaticMeshActor.h"
#include "Kismet/KismetMathLibrary.h"
//#include <cmath>
//#include "Calculation.h"
#include "PaperTest.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Test2.h"

AFieldActor::AFieldActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	// Init Calculator:
	Calculator = CreateDefaultSubobject<UPaperTest>(TEXT("Calculator"));
	_calculator = Calculator;

	#pragma region Creating vector field

	VectorField = CreateDefaultSubobject<UMeshVectorField>(TEXT("Mesh_vector_field"));
	VectorField->Init(&Calculator, &SizeMultipiler);
	VectorField->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	#pragma endregion

	CubeCenter = (Calculator->UpperLimits + Calculator->LowerLimits) / 2;
#if WITH_EDITOR
	SetPivotOffset(CubeCenter);
#endif

#pragma region Creating cuboid surface

	//CuboidSurface = UCuboidSurface::Construct(Calculator->LowerLimits, Calculator->UpperLimits);
	CuboidSurface = CreateDefaultSubobject<UCuboidSurface>(TEXT("CuboidSurface"));
	CuboidSurface->Init(Calculator->LowerLimits, Calculator->UpperLimits);
	CuboidSurface->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
#pragma endregion


#pragma region Creating spline field

	SplineField = CreateDefaultSubobject<USplineField>(TEXT("SplineField"));
	TArray<ISplinesStartArea*> splinesStartAreas;
	splinesStartAreas.Add(CuboidSurface);
	SplineField->Init(&Calculator, splinesStartAreas, &SizeMultipiler);
	SplineField->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

#pragma endregion
}

void AFieldActor::PostLoad()	// Вызывается при загрузке.
{
	Super::PostLoad();
}

void AFieldActor::PostActorCreated()	// Вызывается при созданни в редакторе или запуске мира.
{
	Super::PostActorCreated();
}

void AFieldActor::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

	SetCalculator(NewObject<UPaperTest>());	// Костыль, т.к. калькулятор сбрасывается в NULL.
	
	SetSizeMultipiler(GetActorScale3D().X);
	CubeCenter = (Calculator->UpperLimits + Calculator->LowerLimits) / 2;
#if WITH_EDITOR
	SetPivotOffset(CubeCenter * GetSizeMultipiler());
#endif

	UCuboidFace* cuboidFace = CuboidSurface->GetFaceBy(FaceAxis::XY, FacePosition::Front);
	cuboidFace->IsActivated = true;
	SplineField->UpdateSplines(0, true);
	VectorField->Revisualize();
}

#pragma region Setters for uproperties

void AFieldActor::SetSimulationTime(float newSimulationTime)
{
	float deltaTime = newSimulationTime - SimulationTime;
	SimulationTime = newSimulationTime;	// TODO: move to Calculator.

	Calculator->Time = SimulationTime;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::SanitizeFloat(SimulationTime));
	
	VectorField->VectorMaterial->SetScalarParameterValue(TEXT("time"), SimulationTime);		// TODO: move to Revisualize()?
	SplineField->SplineMaterial->SetScalarParameterValue(TEXT("time"), SimulationTime);		// TODO: move to UpdateSpline()?

	SplineField->UpdateSplines(deltaTime);
}

void AFieldActor::SetEpsilon(float epsilon)
{
	Calculator->Epsilon = epsilon;

	SplineField->SplineMaterial->SetScalarParameterValue(TEXT("epsilon"), epsilon);
	SplineField->UpdateSplines(0);

	VectorField->VectorMaterial->SetScalarParameterValue(TEXT("epsilon"), epsilon);
}

void AFieldActor::SetSizeMultipiler(float sizeMultipiler)
{
	SizeMultipiler = sizeMultipiler;
	SetActorRelativeScale3D(FVector(SizeMultipiler, SizeMultipiler, SizeMultipiler));

	SplineField->SplineMaterial->SetScalarParameterValue(TEXT("scale"), SizeMultipiler);
	VectorField->VectorMaterial->SetScalarParameterValue(TEXT("scale"), SizeMultipiler);
}

//void AFieldActor::SetIsShowVectors(bool isShowVectors)
//{
//	IsShowVectors = isShowVectors;
//
//	if (IsShowVectors)
//	{
//		//_reCreateVecotrField();
//	}
//	else
//	{
//		//VectorInstancedMesh->ClearInstances();
//	}
//	//VectorInstancedMesh->MarkRenderStateDirty();	// for debug
//}

void AFieldActor::SetLyambda(float lyambda)
{
	Calculator->Lyambda = lyambda;
	SplineField->SplineMaterial->SetScalarParameterValue(TEXT("lyambda"), lyambda);
	SplineField->UpdateSplines(0);

	VectorField->VectorMaterial->SetScalarParameterValue(TEXT("lyambda"), lyambda);
}

void AFieldActor::SetLowerLimits(FVector newLowerLimits)
{
	Calculator->LowerLimits = newLowerLimits;
	
	CuboidSurface->UpdateSurface(Calculator->LowerLimits, Calculator->UpperLimits);
	SplineField->UpdateSplines(0, true);

	VectorField->Revisualize();
}

void AFieldActor::SetUpperLimits(FVector newUpperLimits)
{
	Calculator->UpperLimits = newUpperLimits;

	CuboidSurface->UpdateSurface(Calculator->LowerLimits, Calculator->UpperLimits);
	SplineField->UpdateSplines(0, true);

	VectorField->Revisualize();
}

void AFieldActor::SetIsShowSplines(bool isShowSplines)
{
	IsShowSplines = isShowSplines;

	if (IsShowSplines)
	{
		//UpdateSpline();
	}
	else
	{
		//SplineInstancedMesh->ClearInstances();
	}
}
#pragma endregion

#pragma region Getters for uproperties

float AFieldActor::GetSimulationTime()
{
	return SimulationTime;
}

void AFieldActor::SetCalculator(UCalculator* calculator)
{
	Calculator = calculator;
	_calculator = Calculator;

	FString name;
	Calculator->GetClass()->GetName(name);
	SplineField->SelectMaterial(name);
	SplineField->UpdateSplines(0, true);

	VectorField->Revisualize();
}

float AFieldActor::GetEpsilon()
{
	return Calculator->Epsilon;
}

float AFieldActor::GetSizeMultipiler()
{
	return GetActorScale3D().X;
}

bool AFieldActor::GetIsShowVectors()
{
	return IsShowVectors;
}

float AFieldActor::GetLyambda()
{
	return Calculator->Lyambda;
}

FVector AFieldActor::GetLowerLimits()
{
	return Calculator->LowerLimits;
}

FVector AFieldActor::GetUpperLimits()
{
	return Calculator->UpperLimits;
}

bool AFieldActor::GetIsShowSplines()
{
	return IsShowSplines;
}
#pragma endregion


#if WITH_EDITOR
void AFieldActor::PostEditChangeProperty(FPropertyChangedEvent& e)	// TODO: сделать сеттеры для свойств.
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.MemberProperty->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, EditorFlipFlopFieldsUpdate))
	{
		SplineField->UpdateSplines(0);
		VectorField->Revisualize();
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, IsShowSplines))
	{
		SetIsShowSplines(IsShowSplines);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, SimulationTime))
	{
		SetSimulationTime(SimulationTime);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(AFieldActor, Calculator))
	{
		SetCalculator(Calculator);
	}
}
#endif

void AFieldActor::BeginPlay()
{
	Super::BeginPlay();

	SplineField->FillSplineWithParticles(1);
}

void AFieldActor::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);
}