// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnBox.h"

#include "HW08Character.h"
#include "RespawnFloor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

class AHW08Character;
// Sets default values
ARespawnBox::ARespawnBox()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	ReSpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ReSpawnBox"));
	ReSpawnBox->SetCollisionProfileName("OverlapAllDynamic");
	ReSpawnBox->SetupAttachment(Scene);

	ReSpawnBox->OnComponentBeginOverlap.AddDynamic(this, &ARespawnBox::OnBoxOverlap);
	ReSpawnBox->OnComponentEndOverlap.AddDynamic(this, &ARespawnBox::OnBoxEndOverlap);

	RespawnDamage = 30.0f;
}

void ARespawnBox::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		PlayerRespawn();
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Green,
			FString::Printf(TEXT("Player Respawn!!!")));
	}
}

void ARespawnBox::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ARespawnBox::PlayerRespawn()
{
	TArray<AActor*> RespawnFloors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnFloor::StaticClass(), RespawnFloors);
    
	if (RespawnFloors.Num() > 0)
	{
		// 첫 번째 RespawnFloor의 위치 가져오기
		if (ARespawnFloor* RespawnPoint = Cast<ARespawnFloor>(RespawnFloors[0]))
		{
			// 플레이어 찾기
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

			if (AHW08Character* PlayerCharacter = Cast<AHW08Character>(PlayerController->GetPawn()))
			{
				if (PlayerCharacter)
				{
					RespawnPoint->SetFloor();
					// RespawnFloor 위치에 플레이어 텔레포트
					FVector SpawnLocation = RespawnPoint->GetActorLocation();
					// 바닥 위로 약간 올려서 스폰
					SpawnLocation.Z += 200.0f;
			
					PlayerCharacter->SetActorLocation(SpawnLocation);

					UGameplayStatics::ApplyDamage(
						PlayerCharacter,            // 데미지를 받을 액터
						RespawnDamage,              // 데미지 양
						nullptr,                    // 데미지를 유발한 주체 (지뢰를 설치한 캐릭터가 없으므로 nullptr)
						this,                       // 데미지를 유발한 오브젝트(지뢰)
						UDamageType::StaticClass()  // 기본 데미지 유형
					);
				}
			}
		}
	}
}



