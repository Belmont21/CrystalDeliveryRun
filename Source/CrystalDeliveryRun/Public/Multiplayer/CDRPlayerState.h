#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CDRPlayerState.generated.h"

class ACDRCrystal;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCrystalScoreChanged, int32);

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	FOnCrystalScoreChanged OnCrystalScoreChanged;

	void RegisterCrystalDelivery(const ACDRCrystal* Crystal);
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_CrystalScore, BlueprintReadOnly, Category = "Crystal Delivery")
	int32 CrystalScore = 0;

	UFUNCTION()
	void OnRep_CrystalScore();
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crystal Delivery")
	int32 CommonDelivered = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crystal Delivery")
	int32 RareDelivered = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crystal Delivery")
	int32 LegendaryDelivered = 0;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crystal Delivery")
	int32 FallCount = 0;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crystal Delivery")
	int32 DeathCount = 0;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Crystal Delivery")
	FName PlayerRole = TEXT("Recolector");


};