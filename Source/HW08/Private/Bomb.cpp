// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"
#include "Floor.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

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

	UParticleSystemComponent* Particle = nullptr;
	if (BombSpawnParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BombSpawnParticle,
			GetActorLocation(),
			GetActorRotation(),
			true);
	}
	
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				Particle->DestroyComponent();
			},
			5.0f,
			false			
		);
	}
	if (BombSpawnSound)
	{
		if (UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), BombSpawnSound))
		{
			// 4초 후에 사운드 정지
			FTimerHandle SoundTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				SoundTimerHandle,
				[AudioComp]()
				{
					if (AudioComp && AudioComp->IsValidLowLevel())
					{
						AudioComp->Stop();
					}
				},
				4.0f,
				false
			);
		}

	}
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
	UParticleSystemComponent* Particle = nullptr;
	if (BombParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BombParticle,
			GetActorLocation(),
			GetActorRotation(),
			true);
	}
	if (BombSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			BombSound,
			GetActorLocation()
		);
	}
	
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				Particle->DestroyComponent();
			},
			3.0f,
			false			
		);
	}
	
	for (auto Floor : OverlappedFloors)
	{
		Floor->DestroyFloor();
	}
	Destroy();
}



