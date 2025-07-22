// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnFloor.generated.h"

class UBoxComponent;

UCLASS()
class HW08_API ARespawnFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnFloor();

	void HiddenFloor();
	void ShowFloor();
	void SetFloor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ReSpawnBox")
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ReSpawnBox")
	TObjectPtr<UBoxComponent> ReSpawnBox;

};
