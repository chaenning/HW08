// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

class AFloor;
class USphereComponent;

UCLASS()
class HW08_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomb();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<USceneComponent> Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<USphereComponent> Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* BombParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* BombSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* BombSpawnParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* BombSpawnSound;

	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY()
	TArray<AFloor*> OverlappedFloors;


	FTimerHandle BombTimerHandle;
	float DestroyTime;
	
	void DestroyBomb();

	UFUNCTION(BlueprintCallable, Category = "Bomb")
	void AddFloor(AActor* OtherActor);
	UFUNCTION(BlueprintCallable, Category = "Bomb")
	void RemoveFloor(AActor* OtherActor);
};
