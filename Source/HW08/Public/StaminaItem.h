// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "StaminaItem.generated.h"

/**
 * 
 */
UCLASS()
class HW08_API AStaminaItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AStaminaItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 StaminaAmount;
	
	virtual void ActivateItem(AActor* Activator) override;
	
};
