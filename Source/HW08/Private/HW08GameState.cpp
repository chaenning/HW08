#include "HW08GameState.h"
#include "CoinItem.h"
#include "HW08Character.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "HW08GameInstance.h"
#include "HW08PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

AHW08GameState::AHW08GameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	CurrentWaveindex = 0;
	MaxLevels = 3;
	bIsWave = false;
}

void AHW08GameState::BeginPlay()
{
	Super::BeginPlay();
	
	StartLevel();
	
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AHW08GameState::UpdateHUD,
		0.1f,
		true);
}

int32 AHW08GameState::GetScore() const
{
	return Score;
}

void AHW08GameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHW08GameInstance* HW08GameInstance = Cast<UHW08GameInstance>(GameInstance);
		if (HW08GameInstance)
		{
			HW08GameInstance->AddToScore(Amount);
		}
	}
}

void AHW08GameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHW08PlayerController* HW08PlayerController = Cast<AHW08PlayerController>(PlayerController))
		{
			HW08PlayerController->ShowGameHUD();
		}
	}
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHW08GameInstance* HW08GameInstance = Cast<UHW08GameInstance>(GameInstance);
		if (HW08GameInstance)
		{
			CurrentLevelIndex = HW08GameInstance->CurrentLevelIndex;
		}
	}
	
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	// 스폰된 코인 갯수
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor &&SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
	
	GetWorldTimerManager().SetTimer(
			LevelTimerHandle,
			this,
			&AHW08GameState::OnLevelTimeUp,
			LevelDuration,
			false
		);
	
	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"),
		CurrentLevelIndex + 1,
		SpawnedCoinCount);
}

void AHW08GameState::OnLevelTimeUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Level Time Up!"));
	EndLevel();
}

void AHW08GameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"), 
	CollectedCoinCount,
	SpawnedCoinCount)
	
	// 현재 레벨에서 스폰된 코인을 전부 주웠다면 즉시 레벨 종료
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}


void AHW08GameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);


	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHW08GameInstance* HW08GameInstance = Cast<UHW08GameInstance>(GameInstance);
		if (HW08GameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			HW08GameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	
	if (CurrentLevelIndex > MaxLevels)
	{
		UE_LOG(LogTemp, Warning, TEXT("MaxLevel!"));
		OnGameOver();
		return;
	}
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		// 맵 이름이 없으면 게임오버
		UE_LOG(LogTemp, Warning, TEXT("Level Map Name is Invalid!"));
		OnGameOver();
	}
}
void AHW08GameState::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("=== OnGameOver Debug ==="));
	UE_LOG(LogTemp, Warning, TEXT("CurrentLevelIndex: %d"), CurrentLevelIndex);
	UE_LOG(LogTemp, Warning, TEXT("LevelMapNames Size: %d"), LevelMapNames.Num());

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHW08PlayerController* HW08PlayerController = Cast<AHW08PlayerController>(PlayerController))
		{
			HW08PlayerController->SetPause(true);
			HW08PlayerController->ShowMainMenu(true);
		}
	}
}

void AHW08GameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHW08PlayerController* HW08PlayerController = Cast<AHW08PlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = HW08PlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime =  GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UHW08GameInstance* HW08GameInstance = Cast<UHW08GameInstance>(GameInstance);
						if (HW08GameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), HW08GameInstance->TotalScore)));
						}
					}					
				}
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex+1)));
				}
				if (UTextBlock* CoinText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Coin"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{						
						CoinText->SetText(FText::FromString(FString::Printf(TEXT("Coin: %d/%d"), CollectedCoinCount, SpawnedCoinCount)));
					}					
				}
				if (UTextBlock* PotionText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Potion"))))
				{
					if (UImage* ImageWidget = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("PotionImage"))))
					{
						if (AHW08Character* PlayerCharacter = Cast<AHW08Character>(PlayerController->GetPawn()))
						{
							if (PlayerCharacter->GetIsBoost())
							{
								ImageWidget->SetVisibility(ESlateVisibility::Visible);
								PotionText->SetText(FText::FromString(
								FString::Printf(TEXT("%.0f"), PlayerCharacter->GetRemainBoostTime())));
							}
							else
							{
								ImageWidget->SetVisibility(ESlateVisibility::Hidden);
								PotionText->SetText(FText::FromString(FString::Printf(TEXT(""))));
							}
						}				
					}						
				}
				//스태미나
				if (UTextBlock* StaminaText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Stamina"))))
				{
					if (AHW08Character* PlayerCharacter = Cast<AHW08Character>(PlayerController->GetPawn()))
					{
						StaminaText->SetText(FText::FromString(
						FString::Printf(TEXT("%.0f / %.0f"), PlayerCharacter->GetStamina(), PlayerCharacter->GetMaxStamina())));
					}					
				}
				if (UProgressBar* StaminaBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("StaminaBar"))))
				{
					if (AHW08Character* PlayerCharacter = Cast<AHW08Character>(PlayerController->GetPawn()))
					{
						StaminaBar->SetPercent(PlayerCharacter->GetStamina()/ PlayerCharacter->GetMaxStamina());
					}
				}
				if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{						
						WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveindex +1)));
					}					
				}
				if (UTextBlock* BombText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Bomb"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
						int32 TimeInt = FMath::FloorToInt(RemainingTime);
						static int32 LastCheckedTime = -1;
        
						if (!bIsWave && (TimeInt == 20 || TimeInt == 12) && TimeInt != LastCheckedTime)
						{
							// Wave 관련 로직
							UpdateWave();
            
							// UI 업데이트
							BombText->SetText(FText::FromString(TEXT("폭탄이 설치되었습니다.")));
            
							// 약한 포인터로 캡처
							TWeakObjectPtr<UTextBlock> WeakBombText(BombText);
            
							// 타이머 설정
							FTimerHandle TextTimerHandle;
							GetWorld()->GetTimerManager().SetTimer(
								TextTimerHandle,
								[WeakBombText]()
								{
									if (WeakBombText.IsValid())
									{
										WeakBombText->SetText(FText::FromString(TEXT("")));
									}
								},
								3.0f,
								false
							);
            
							CurrentWaveindex++;
							LastCheckedTime = TimeInt;
						}			
					}					
				}
			}
		} 
	}
}

void AHW08GameState::UpdateWave()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateWave"));
	TArray<AActor*> SpawnVolume;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), SpawnVolume);

	if (SpawnVolume.Num() > 0)
	{
		// 첫 번째 RespawnFloor의 위치 가져오기
		if (ASpawnVolume* SpawnBomb = Cast<ASpawnVolume>(SpawnVolume[0]))
		{
			for (int32 i = 0; i < (CurrentLevelIndex + 1) * 3 ; i++)
			{
				SpawnBomb->SpawnBomb();
			}
		}
	}
}
