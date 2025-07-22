// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnBox.generated.h"

class UBoxComponent;

UCLASS()
class HW08_API ARespawnBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnBox();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ReSpawnBox")
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ReSpawnBox")
	TObjectPtr<UBoxComponent> ReSpawnBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ReSpawnBox")
	float RespawnDamage;

	UFUNCTION()
	virtual void OnBoxOverlap(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnBoxEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void PlayerRespawn();

};
