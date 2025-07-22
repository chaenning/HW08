// Fill out your copyright notice in the Description page of Project Settings.


#include "ReverseItem.h"
#include "HW08Character.h"

AReverseItem::AReverseItem()
{
	ItemType = "Reserve";
	FastAmount = 1.5f;
	ReverseDuration = 5.0f;
}

void AReverseItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AHW08Character* PlayerCharacter = Cast<AHW08Character>(Activator))
		{
			PlayerCharacter->BoostSpeed(FastAmount, ReverseDuration);
		}        
		DestroyItem();
	}
}
