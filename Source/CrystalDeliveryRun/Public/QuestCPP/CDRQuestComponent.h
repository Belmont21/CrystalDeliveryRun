#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestCPP/CDRQuestTypes.h"
#include "CDRQuestComponent.generated.h"

class UCDRQuestDefinition;
class ACDRQuestGameState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FCDRQuestRuntimeEvent,
	FCDRQuestRuntime,
	QuestData);

UCLASS(ClassGroup = (Quest), meta = (BlueprintSpawnableComponent))
class CRYSTALDELIVERYRUN_API UCDRQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCDRQuestComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FCDRQuestRuntime> ActiveQuests;

	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FCDRQuestRuntimeEvent OnQuestStarted;

	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FCDRQuestRuntimeEvent OnQuestProgressChanged;

	UPROPERTY(BlueprintAssignable, Category = "Quest|Events")
	FCDRQuestRuntimeEvent OnQuestCompleted;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool AddQuest(UCDRQuestDefinition* QuestDefinition);

	UFUNCTION(BlueprintPure, Category = "Quest")
	bool FindQuestByID(
		FName QuestID,
		FCDRQuestRuntime& OutQuestData,
		int32& OutQuestIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void HandleGameplayObjective(
		ECDRQuestObjectiveType ObjectiveType,
		FName TargetID,
		int32 Amount);

private:
	void UpdateObjectiveProgress(
		int32 QuestIndex,
		int32 ObjectiveIndex,
		int32 Amount);

	void CheckQuestCompleted(int32 QuestIndex);
};