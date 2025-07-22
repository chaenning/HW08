// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaItem.h"
#include "HW08Character.h"

AStaminaItem::AStaminaItem()
{
	StaminaAmount = 20;
	ItemType = "Stamina";
}

void AStaminaItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AHW08Character* PlayerCharacter = Cast<AHW08Character>(Activator))
		{
			// 캐릭터의 체력을 회복
			PlayerCharacter->AddStamina(StaminaAmount);
		}        
		DestroyItem();
	}
}
