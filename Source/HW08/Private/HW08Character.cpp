#include "HW08Character.h"
#include "HW08PlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHW08Character::AHW08Character()
{ 	
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;
	
	NormalSpeed = 500.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}


void AHW08Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AHW08PlayerController* PlayerController = Cast<AHW08PlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AHW08Character::Move
				);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AHW08Character::StartJump
				);
				
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AHW08Character::StopJump
				);
			}
            
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AHW08Character::Look
				);
			}
            
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered, 
					this, 
					&AHW08Character::StartSprint
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction, 
					ETriggerEvent::Completed, 
					this, 
					&AHW08Character::StopSprint
				);
			}    
		}
	}
}

void AHW08Character::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		// 캐릭터가 바라보는 방향(정면)으로 X축 이동
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		// 캐릭터의 오른쪽 방향으로 Y축 이동
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void AHW08Character::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void AHW08Character::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void AHW08Character::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void AHW08Character::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AHW08Character::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

