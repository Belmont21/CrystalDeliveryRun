#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestCPP/CDRQuestTypes.h"
#include "CDRQuestDefinition.generated.h"

UCLASS(BlueprintType)
class CRYSTALDELIVERYRUN_API UCDRQuestDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName QuestID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FCDRQuestObjective> Objectives;
};