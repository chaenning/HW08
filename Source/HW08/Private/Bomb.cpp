// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "Floor.h"
#include "Components/SphereComponent.h"

// Sets default values
ABomb::ABomb()
{
   PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(StaticMesh);

	if (StaticMesh)
	{
		StaticMesh->SetSimulatePhysics(true);
		StaticMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
		StaticMesh->SetGenerateOverlapEvents(true);
	}
	
	if (Collision)
	{
		Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		Collision->SetGenerateOverlapEvents(true);
	}

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABomb::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABomb::OnItemEndOverlap);
	
	DestroyTime = 5.0f;
}

// Called when the game starts or when spawned
void ABomb::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
			BombTimerHandle,
			this,
			&ABomb::DestroyBomb,
			DestroyTime,
			false 
		);
}

void ABomb::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ABomb::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
}

void ABomb::AddFloor(AActor* OtherActor)
{
	if (OtherActor && OtherActor->ActorHasTag("Floor"))
	{
		if (AFloor* FloorActor = Cast<AFloor>(OtherActor))
		{
			// Floor 배열에 추가
			OverlappedFloors.AddUnique(FloorActor);
			UE_LOG(LogTemp, Warning, TEXT("Floor Added to overlap list"));
		}
	}
}

void ABomb::RemoveFloor(AActor* OtherActor)
{
	if (OtherActor && OtherActor->ActorHasTag("Floor"))
	{
		if (AFloor* FloorActor = Cast<AFloor>(OtherActor))
		{
			// Floor 배열에서 제거
			OverlappedFloors.Remove(FloorActor);
			UE_LOG(LogTemp, Warning, TEXT("Floor Removed from overlap list"));
		}
	}
}


void ABomb::DestroyBomb()
{
	for (auto Floor : OverlappedFloors)
	{
		Floor->DestroyFloor();
	}
	Destroy();
}



