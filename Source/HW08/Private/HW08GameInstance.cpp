// Fill out your copyright notice in the Description page of Project Settings.


#include "HW08GameInstance.h"

UHW08GameInstance::UHW08GameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UHW08GameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}
