#include "QuestCPP/CDRQuestComponent.h"
#include "QuestCPP/CDRQuestGameState.h"
#include "QuestCPP/CDRQuestDefinition.h"

UCDRQuestComponent::UCDRQuestComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}
void UCDRQuestComponent::BeginPlay()
{
    Super::BeginPlay();

    ACDRQuestGameState* QuestGameState =
        GetWorld()->GetGameState<ACDRQuestGameState>();

    if (IsValid(QuestGameState))
    {
        QuestGameState->OnGameplayObjectiveEvent.AddDynamic(
            this,
            &UCDRQuestComponent::HandleGameplayObjective);
    }
}
bool UCDRQuestComponent::FindQuestByID(
    const FName QuestID,
    FCDRQuestRuntime& OutQuestData,
    int32& OutQuestIndex) const
{
    OutQuestData = FCDRQuestRuntime();
    OutQuestIndex = INDEX_NONE;

    for (int32 Index = 0; Index < ActiveQuests.Num(); ++Index)
    {
        if (ActiveQuests[Index].QuestID == QuestID)
        {
            OutQuestData = ActiveQuests[Index];
            OutQuestIndex = Index;
            return true;
        }
    }

    return false;
}

bool UCDRQuestComponent::AddQuest(UCDRQuestDefinition* QuestDefinition)
{
    if (!IsValid(QuestDefinition))
    {
        return false;
    }

    FCDRQuestRuntime ExistingQuest;
    int32 ExistingQuestIndex = INDEX_NONE;

    if (FindQuestByID(
        QuestDefinition->QuestID,
        ExistingQuest,
        ExistingQuestIndex))
    {
        return false;
    }

    FCDRQuestRuntime NewQuest;
    NewQuest.QuestID = QuestDefinition->QuestID;
    NewQuest.QuestDefinition = QuestDefinition;
    NewQuest.Objectives = QuestDefinition->Objectives;
    NewQuest.bIsActive = true;
    NewQuest.bIsCompleted = false;

    for (FCDRQuestObjective& Objective : NewQuest.Objectives)
    {
        Objective.CurrentAmount = 0;
    }

    ActiveQuests.Add(NewQuest);
    OnQuestStarted.Broadcast(NewQuest);

    return true;
}

void UCDRQuestComponent::HandleGameplayObjective(
    const ECDRQuestObjectiveType ObjectiveType,
    const FName TargetID,
    const int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }

    for (int32 QuestIndex = 0; QuestIndex < ActiveQuests.Num(); ++QuestIndex)
    {
        const FCDRQuestRuntime& Quest = ActiveQuests[QuestIndex];

        if (!Quest.bIsActive || Quest.bIsCompleted)
        {
            continue;
        }

        for (int32 ObjectiveIndex = 0;
            ObjectiveIndex < Quest.Objectives.Num();
            ++ObjectiveIndex)
        {
            const FCDRQuestObjective& Objective =
                Quest.Objectives[ObjectiveIndex];

            const bool bMatchesObjective =
                Objective.ObjectiveType == ObjectiveType &&
                Objective.TargetID == TargetID;

            if (bMatchesObjective)
            {
                UpdateObjectiveProgress(
                    QuestIndex,
                    ObjectiveIndex,
                    Amount);
            }
        }
    }
}

void UCDRQuestComponent::UpdateObjectiveProgress(
    const int32 QuestIndex,
    const int32 ObjectiveIndex,
    const int32 Amount)
{
    if (!ActiveQuests.IsValidIndex(QuestIndex))
    {
        return;
    }

    FCDRQuestRuntime& Quest = ActiveQuests[QuestIndex];

    if (!Quest.Objectives.IsValidIndex(ObjectiveIndex))
    {
        return;
    }

    FCDRQuestObjective& Objective = Quest.Objectives[ObjectiveIndex];

    const int32 PreviousAmount = Objective.CurrentAmount;
    Objective.CurrentAmount = FMath::Clamp(
        Objective.CurrentAmount + Amount,
        0,
        Objective.RequiredAmount);

    if (Objective.CurrentAmount == PreviousAmount)
    {
        return;
    }

    OnQuestProgressChanged.Broadcast(Quest);
    CheckQuestCompleted(QuestIndex);
}

void UCDRQuestComponent::CheckQuestCompleted(const int32 QuestIndex)
{
    if (!ActiveQuests.IsValidIndex(QuestIndex))
    {
        return;
    }

    FCDRQuestRuntime& Quest = ActiveQuests[QuestIndex];

    if (Quest.bIsCompleted || Quest.Objectives.IsEmpty())
    {
        return;
    }

    for (const FCDRQuestObjective& Objective : Quest.Objectives)
    {
        if (Objective.CurrentAmount < Objective.RequiredAmount)
        {
            return;
        }
    }

    Quest.bIsActive = false;
    Quest.bIsCompleted = true;

    OnQuestCompleted.Broadcast(Quest);
}
