// Fill out your copyright notice in the Description page of Project Settings.


#include "HW08Character.h"

// Sets default values
AHW08Character::AHW08Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHW08Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHW08Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHW08Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

