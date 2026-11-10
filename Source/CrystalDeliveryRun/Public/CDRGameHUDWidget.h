#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "CDRGameHUDWidget.generated.h"

class UTextBlock;
class UButton;
class UProgressBar;

UCLASS()
class CRYSTALDELIVERYRUN_API UCDRGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetScore(int32 NewScore);
	void SetRemainingTime(int32 Seconds);
	void ShowMatchResult(const FText& Result);
	void ShowCrystalLost();
	void ShowCrystalDelivered(int32 Points);
	void SetHealth(float NewHealth);
	void SetTotalScore(int32 NewTotalScore);
	void SetLobbyState(bool bIsLobby, bool bIsHost);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	void HideCrystalLost();

	UFUNCTION()
	void HandleExitClicked();

	UFUNCTION()
	void HandleRestartClicked();
	
	UFUNCTION()
	void HandleStartClicked();

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> LobbyText;

	UPROPERTY(Transient)
	TObjectPtr<UButton> StartButton;
	
	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> OutcomeText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> CrystalLostText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> ScoreText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> TimerText;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> ResultText;

	UPROPERTY(Transient)
	TObjectPtr<UButton> RestartButton;
	
	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> TotalScoreText;

	UPROPERTY(Transient)
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(Transient)
	TObjectPtr<UProgressBar> HealthBar;

	FTimerHandle CrystalLostTimerHandle;
};

