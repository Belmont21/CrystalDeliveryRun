#include "Multiplayer/CDRPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "CDRCrystal.h"
#include "Engine/World.h"
#include "Multiplayer/CDRMatchGameState.h"

void ACDRPlayerState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACDRPlayerState, CrystalScore);
	DOREPLIFETIME(ACDRPlayerState, CommonDelivered);
	DOREPLIFETIME(ACDRPlayerState, RareDelivered);
	DOREPLIFETIME(ACDRPlayerState, LegendaryDelivered);
	DOREPLIFETIME(ACDRPlayerState, FallCount);
	DOREPLIFETIME(ACDRPlayerState, DeathCount);
	DOREPLIFETIME(ACDRPlayerState, PlayerRole);
}
void ACDRPlayerState::RegisterCrystalDelivery(
	const ACDRCrystal* Crystal)
{
	if (!HasAuthority() || !IsValid(Crystal))
	{
		return;
	}

	CrystalScore += Crystal->GetPointValue();
	if (UWorld* World = GetWorld())
	{
		if (ACDRMatchGameState* MatchGameState =
		   World->GetGameState<ACDRMatchGameState>())
		{
			MatchGameState->AddToTotalScore(Crystal->GetPointValue());
		}
	}

	OnCrystalScoreChanged.Broadcast(CrystalScore);

	switch (Crystal->GetRarity())
	{
	case ECDRCrystalRarity::Common:
		CommonDelivered++;
		break;

	case ECDRCrystalRarity::Rare:
		RareDelivered++;
		break;

	case ECDRCrystalRarity::Legendary:
		LegendaryDelivered++;
		break;
	}
}

void ACDRPlayerState::OnRep_CrystalScore()
{
	OnCrystalScoreChanged.Broadcast(CrystalScore);
}
