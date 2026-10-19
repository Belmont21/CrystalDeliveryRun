#include "QuestCPP/CDRQuestGameState.h"

void ACDRQuestGameState::ReportGameplayObjective(
	const ECDRQuestObjectiveType ObjectiveType,
	const FName TargetID,
	const int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	OnGameplayObjectiveEvent.Broadcast(
		ObjectiveType,
		TargetID,
		Amount);
}