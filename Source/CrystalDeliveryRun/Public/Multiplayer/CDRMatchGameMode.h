#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "CDRMatchGameMode.generated.h"

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACDRMatchGameMode();

	void StartRound();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Match", meta = (ClampMin = "1"))
	int32 RoundDuration = 300;

private:
	void UpdateRoundTimer();
	void CalculateWinners();

	FTimerHandle RoundTimerHandle;
};
