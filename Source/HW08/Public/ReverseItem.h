// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ReverseItem.generated.h"

/**
 * 
 */
UCLASS()
class HW08_API AReverseItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AReverseItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float FastAmount;
	float ReverseDuration;
	
	virtual void ActivateItem(AActor* Activator) override;	
	
};
