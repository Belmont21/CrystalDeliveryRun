#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "QuestCPP/CDRQuestTypes.h"
#include "CDRQuestGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FCDRGameplayObjectiveEvent,
	ECDRQuestObjectiveType,
	ObjectiveType,
	FName,
	TargetID,
	int32,
	Amount);

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRQuestGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FCDRGameplayObjectiveEvent OnGameplayObjectiveEvent;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void ReportGameplayObjective(
		ECDRQuestObjectiveType ObjectiveType,
		FName TargetID,
		int32 Amount = 1);
};