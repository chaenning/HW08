#include "CoinItem.h"

#include "HW08GameState.h"
#include "Engine/World.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";

	PrimaryActorTick.bCanEverTick = true;
	RotationSpeed = 90.0f;
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	// 플레이어 태그 확인
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (AHW08GameState* GameState = World->GetGameState<AHW08GameState>())
			{
				GameState->AddScore(PointValue);	
			}
		}
		DestroyItem();
	}
}


void ACoinItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!FMath::IsNearlyZero(RotationSpeed))
	{
		// 초당 RotationSpeed만큼, 한 프레임당 (RotationSpeed * DeltaTime)만큼 회전
		AddActorLocalRotation(FRotator(0.0f, RotationSpeed * DeltaSeconds, 0.0f));
	}
}


