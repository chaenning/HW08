// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HW08GameState.generated.h"

/**
 * 
 */
UCLASS()
class HW08_API AHW08GameState : public AGameState
{
	GENERATED_BODY()
public:
	AHW08GameState();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
};
