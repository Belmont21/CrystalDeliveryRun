#pragma once

#include "CoreMinimal.h"
#include "CDRQuestTypes.generated.h"

class UCDRQuestDefinition;

UENUM(BlueprintType)
enum class ECDRQuestObjectiveType : uint8
{
	Kill,
	Collect,
	Interact,
	ReachLocation
};

USTRUCT(BlueprintType)
struct FCDRQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName ObjectiveID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	ECDRQuestObjectiveType ObjectiveType = ECDRQuestObjectiveType::Collect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName TargetID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 RequiredAmount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 CurrentAmount = 0;
};

USTRUCT(BlueprintType)
struct FCDRQuestRuntime
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	FName QuestID = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TObjectPtr<UCDRQuestDefinition> QuestDefinition = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FCDRQuestObjective> Objectives;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	bool bIsActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	bool bIsCompleted = false;
};