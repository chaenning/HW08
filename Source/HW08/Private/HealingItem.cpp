// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"

#include "HW08Character.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AHW08Character* PlayerCharacter = Cast<AHW08Character>(Activator))
		{
			// 캐릭터의 체력을 회복
			PlayerCharacter->AddHealth(HealAmount);
		}
        
		DestroyItem();
	}
}
