// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HW08PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class HW08_API AHW08PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHW08PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	void BeginPlay() override;
};
