#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CDRMatchGameState.generated.h"

class ACDRPlayerState;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemainingTimeChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FCDRTotalScoreChanged, int32);

UENUM(BlueprintType)
enum class ECDRMatchPhase : uint8
{
	Lobby,
	Playing,
	Finished
};

DECLARE_MULTICAST_DELEGATE_OneParam(FCDRMatchPhaseChanged, ECDRMatchPhase);

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRMatchGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	
	FCDRMatchPhaseChanged OnMatchPhaseChanged;

	UFUNCTION()
	void OnRep_MatchPhase();
	
	FCDRTotalScoreChanged OnTotalScoreChanged;

	UFUNCTION()
	void OnRep_TotalScore();
	
	FOnRemainingTimeChanged OnRemainingTimeChanged;

	UFUNCTION()
	void OnRep_RemainingTime();

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_MatchPhase, BlueprintReadOnly, Category = "Match")
	ECDRMatchPhase MatchPhase = ECDRMatchPhase::Lobby;
	
	UPROPERTY(ReplicatedUsing = OnRep_RemainingTime, BlueprintReadOnly, Category = "Match")
	int32 RemainingTime = 300;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Match")
	TArray<TObjectPtr<ACDRPlayerState>> Winners;
	
	UPROPERTY(ReplicatedUsing = OnRep_TotalScore, BlueprintReadOnly, Category = "Match")
	int32 TotalScore = 0;
	
	void AddToTotalScore(int32 Points);

};
