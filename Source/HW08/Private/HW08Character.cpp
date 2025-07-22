#include "HW08Character.h"
#include "HW08PlayerController.h"
#include "EnhancedInputComponent.h"
#include "HW08GameState.h"
#include "Camera/CameraComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
	NormalSpeed = 500.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	SpeedBoostMultiplier = 1.0f;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MaxHealth = 100.0f;
	Health = MaxHealth;

	MaxStamina = 100.0f;
	Stamina = 100.0f;
	DiscountStamina = 2.0f;
	IncreaseStamina = 1.0f;
}

void AHW08Character::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
	
	GetWorld()->GetTimerManager().SetTimer(
		StaminaTimerHandle,
		this,
		&AHW08Character::UpdateStamina, 
		0.1f,
		true
	);

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

void AHW08Character::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D MoveInput = Value.Get<FVector2D>();

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

void AHW08Character::StartJump(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		Jump();
	}
}

void AHW08Character::StopJump(const FInputActionValue& Value)
{
	if (!Value.Get<bool>())
	{
		StopJumping();
	}
}

void AHW08Character::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();
	
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void AHW08Character::StartSprint(const FInputActionValue& Value)
{
	if (GetCharacterMovement() && Stamina > 0.0f)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void AHW08Character::StopSprint(const FInputActionValue& Value)
{
	if (GetCharacterMovement())
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

float AHW08Character::GetHealth() const
{
	return Health;
}

void AHW08Character::AddHealth(float Amount)
{
	// 체력을 회복시킴. 최대 체력을 초과하지 않도록 제한함
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

float AHW08Character::GetStamina() const
{
	return Stamina;
}

float AHW08Character::GetMaxStamina() const
{
	return MaxStamina;
}

void AHW08Character::AddStamina(float Amount)
{
	Stamina = FMath::Clamp(Stamina + Amount, 0.0f, MaxStamina);
}


void AHW08Character::UpdateStamina()
{
	if (!bIsSprinting && Stamina >= MaxStamina) return;
	if (bIsSprinting)
	{
		// 스프린트 중일 때 스태미나 감소
		Stamina = FMath::Clamp(Stamina - DiscountStamina, 0.0f, MaxStamina);
		if (Stamina <= 0.0f)
		{
			// 스태미나가 0이 되면 자동으로 스프린트 중지
			StopSprint(FInputActionValue());
		}
	}
	else
	{
		// 스프린트 중이 아닐 때 스태미나 회복
		Stamina = FMath::Clamp(Stamina + IncreaseStamina, 0.0f, MaxStamina);
	}
}

void AHW08Character::BoostSpeed(float Multiplier, float Duration)
{
	if (GetCharacterMovement())
	{
		bIsBoost = true;
		// 현재 진행 중인 스피드 부스트 타이머가 있다면 취소
		if (GetWorld()->GetTimerManager().IsTimerActive(SpeedTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(SpeedTimerHandle);
		}
        
		// 속도 증가
		SpeedBoostMultiplier = Multiplier;
		float NewSpeed = NormalSpeed * SpeedBoostMultiplier;
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
		SprintSpeed = NewSpeed * SprintSpeedMultiplier;
        
		// 지정된 시간 후에 속도를 원래대로 되돌리는 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(
			SpeedTimerHandle,
			this,
			&AHW08Character::ResetSpeed,
			Duration,
			false  // 한 번만 실행
		);
	}
}

bool AHW08Character::GetIsBoost() const
{
	return bIsBoost;
}

float AHW08Character::GetRemainBoostTime()
{
	float RemainingTime = GetWorldTimerManager().GetTimerRemaining(SpeedTimerHandle);
	return RemainingTime;
}

void AHW08Character::ResetSpeed()
{
	if (GetCharacterMovement())
	{
		bIsBoost = false;
		SpeedBoostMultiplier = 1.0f;
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
        
		// 디버그 메시지
		UE_LOG(LogTemp, Warning, TEXT("Speed reset to normal: %f"), NormalSpeed);
	}

}


float AHW08Character::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                 AController* EventInstigator, AActor* DamageCauser)
{	
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 체력을 데미지만큼 감소시키고, 0 이하로 떨어지지 않도록 Clamp
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	// 체력이 0 이하가 되면 사망 처리
	if (Health <= 0.0f)
	{
		OnDeath();
	}

	// 실제 적용된 데미지를 반환
	return ActualDamage;
}

void AHW08Character::OnDeath()
{
	AHW08GameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<AHW08GameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

void AHW08Character::UpdateOverheadHP()
{
	if (!OverheadWidget) return;
	
	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;
	
	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}

