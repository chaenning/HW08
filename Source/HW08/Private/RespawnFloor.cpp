// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnFloor.h"

#include "Components/BoxComponent.h"

// Sets default values
ARespawnFloor::ARespawnFloor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	ReSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ReSpawnBox"));
	ReSpawnBox->SetCollisionProfileName("AllBlockDynamic");
	ReSpawnBox->SetupAttachment(Scene);
}

void ARespawnFloor::HiddenFloor()
{
	SetActorHiddenInGame(true);          // 시각적으로 숨김
	SetActorEnableCollision(false);      // 충돌 비활성화
	SetActorTickEnabled(false);  

}

void ARespawnFloor::ShowFloor()
{
	SetActorHiddenInGame(false);         // 시각적으로 보이게 함
	SetActorEnableCollision(true);       // 충돌 활성화
	SetActorTickEnabled(true);
}

void ARespawnFloor::SetFloor()
{
	ShowFloor();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARespawnFloor::HiddenFloor, 3.0f, false);
}



