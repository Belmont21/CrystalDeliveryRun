#include "Multiplayer/CDRMatchGameState.h"

#include "Net/UnrealNetwork.h"

void ACDRMatchGameState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACDRMatchGameState, MatchPhase);
	DOREPLIFETIME(ACDRMatchGameState, RemainingTime);
	DOREPLIFETIME(ACDRMatchGameState, Winners);
	DOREPLIFETIME(ACDRMatchGameState, TotalScore);
}
void ACDRMatchGameState::OnRep_RemainingTime()
{
	OnRemainingTimeChanged.Broadcast(RemainingTime);
}
void ACDRMatchGameState::AddToTotalScore(int32 Points)
{
	if (!HasAuthority() || Points <= 0)
	{
		return;
	}

	TotalScore += Points;
	OnTotalScoreChanged.Broadcast(TotalScore);
}
void ACDRMatchGameState::OnRep_TotalScore()
{
	OnTotalScoreChanged.Broadcast(TotalScore);
}

void ACDRMatchGameState::OnRep_MatchPhase()
{
	OnMatchPhaseChanged.Broadcast(MatchPhase);
}
