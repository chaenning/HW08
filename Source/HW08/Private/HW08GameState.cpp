// Fill out your copyright notice in the Description page of Project Settings.


#include "HW08GameState.h"

AHW08GameState::AHW08GameState()
{
	Score = 0;
}

int32 AHW08GameState::GetScore() const
{
	return Score;
}

void AHW08GameState::AddScore(int32 Amount)
{	
	Score+= Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}
