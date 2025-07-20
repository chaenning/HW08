// Fill out your copyright notice in the Description page of Project Settings.


#include "HW08GameMode.h"

#include "HW08Character.h"
#include "HW08PlayerController.h"

AHW08GameMode::AHW08GameMode()
{
	DefaultPawnClass = AHW08Character::StaticClass();
	PlayerControllerClass = AHW08PlayerController::StaticClass();
}
