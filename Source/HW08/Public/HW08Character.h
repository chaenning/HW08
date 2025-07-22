#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HW08Character.generated.h"

class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class HW08_API AHW08Character : public ACharacter
{
	GENERATED_BODY()

public:
	AHW08Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> OverheadWidget;

	UFUNCTION(BlueprintPure, Category="Health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
	UFUNCTION(BlueprintPure, Category="Stamina")
	float GetStamina() const;
	UFUNCTION(BlueprintPure, Category="Stamina")
	float GetMaxStamina() const;
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void AddStamina(float Amount);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void BoostSpeed(float Multiplier, float Duration = 5.0f);  // 속도 증가 함수
	bool GetIsBoost() const;
	float GetRemainBoostTime();

	virtual void BeginPlay() override;
protected:
	// 체력 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	// 스태미나 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float DiscountStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float IncreaseStamina;
	bool bIsSprinting;

	
	// 이동 속도 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SpeedBoostMultiplier;
	FTimerHandle SpeedTimerHandle;
	bool bIsBoost;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(	// 데미지 처리 함수 - 외부로부터 데미지를 받을 때 호출됨
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	
	// 사망 처리 함수 (체력이 0 이하가 되었을 때 호출)
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();
	void UpdateOverheadHP();
	
	FTimerHandle StaminaTimerHandle;
	void UpdateStamina();
    
	void ResetSpeed();
};
