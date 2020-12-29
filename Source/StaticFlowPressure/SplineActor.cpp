// Fill out your copyright notice in the Description page of Project Settings.
PRAGMA_DISABLE_OPTIMIZATION

#include "SplineActor.h"

//#include "Components/SplineComponent.h"
//#include "Components/SplineMeshComponent.h"		// ??
//#include "Components/StaticMeshComponent.h"		// ??
//#include "Engine/Engine.h"		// ??

// Sets default values
ASplineActor::ASplineActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	auto root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(root);

	//SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	//SplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	Asset = MeshAsset.Object;
}

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#pragma region creating spline component
	/* 
	Решение по динамическому созданию компонентов вне конструктора взято отсюда: 
	https://forums.unrealengine.com/development-discussion/c-gameplay-programming/13196-construction-scripting-in-c-addstaticmeshcomponent
	и обновлений для новых версий движка:
	https://answers.unrealengine.com/questions/188898/how-can-i-add-components-in-the-onconstruction-fun.html
	*/

	on++;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("OnConstruction(), on = ") + FString::FromInt(on)));

	if (SplineComponent == 0) 
	{
		i++;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("IF == TRUE, i = ") + FString::FromInt(i)));

		SplineComponent = NewObject<USplineComponent>(this);
		SplineComponent->RegisterComponent();
		SplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		// required for proper GC - this causes the mesh to get automatically destroyed when OnConstruction is re-run
		SplineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		//RegisterAllComponents();
	}

	// Mesh
	/*MeshComponent = NewObject<UStaticMeshComponent>(this);
	MeshComponent->SetStaticMesh(Asset);
	
	MeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MeshComponent->RegisterComponent();*/
	// /

#pragma endregion
}

#if WITH_EDITOR
void ASplineActor::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Adding spline component in PostEditChangeProperty()"));

	FName PropertyName = (e.Property != NULL) ? e.MemberProperty->GetFName() : NAME_None;

	SplineComponent = NewObject<USplineComponent>(this);
	SplineComponent->RegisterComponent();
	SplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// required for proper GC - this causes the mesh to get automatically destroyed when OnConstruction is re-run
	//SplineComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
}
#endif

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}