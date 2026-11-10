#include "Multiplayer/CDRHazardZone.h"

#include "Components/BoxComponent.h"
#include "CrystalDeliveryRunCharacter.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Multiplayer/CDRMatchGameState.h"
#include "Multiplayer/CDRPlayerState.h"
#include "CrystalDeliveryRunPlayerController.h"

ACDRHazardZone::ACDRHazardZone()
{
	PrimaryActorTick.bCanEverTick = false;

	HazardBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HazardBox"));
	RootComponent = HazardBox;

	HazardBox->SetBoxExtent(FVector(1000.0f, 1000.0f, 100.0f));
	HazardBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HazardBox->SetCollisionObjectType(ECC_WorldDynamic);
	HazardBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HazardBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HazardBox->SetGenerateOverlapEvents(true);
	HazardBox->OnComponentBeginOverlap.AddDynamic(
   this,
   &ACDRHazardZone::HandleHazardOverlap);

}

void ACDRHazardZone::HandleHazardOverlap(
   UPrimitiveComponent* OverlappedComponent,
   AActor* OtherActor,
   UPrimitiveComponent* OtherComp,
   int32 OtherBodyIndex,
   bool bFromSweep,
   const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	ACDRMatchGameState* MatchGameState =
	   GetWorld()->GetGameState<ACDRMatchGameState>();

	if (!IsValid(MatchGameState) ||
		MatchGameState->MatchPhase != ECDRMatchPhase::Playing)
	{
		return;
	}

	ACrystalDeliveryRunCharacter* Character =
	   Cast<ACrystalDeliveryRunCharacter>(OtherActor);

	if (!IsValid(Character))
	{
		return;
	}

	APlayerController* PlayerController =
	   Cast<APlayerController>(Character->GetController());

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

	if (!IsValid(PlayerController) || !IsValid(GameMode))
	{
		return;
	}

	if (ACDRPlayerState* CDRPlayerState =
   PlayerController->GetPlayerState<ACDRPlayerState>())
	{
		CDRPlayerState->FallCount++;
		CDRPlayerState->DeathCount++;
	}

	if (Character->IsCarryingCrystal())
	{
		ACrystalDeliveryRunPlayerController* CDRPlayerController =
		   Cast<ACrystalDeliveryRunPlayerController>(PlayerController);

		if (IsValid(CDRPlayerController))
		{
			CDRPlayerController->ClientCrystalLost();
		}
	}

	ACrystalDeliveryRunPlayerController* FallSoundController =
   Cast<ACrystalDeliveryRunPlayerController>(PlayerController);

	if (IsValid(FallSoundController))
	{
		FallSoundController->ClientPlayFallSound();
	}

	Character->LoseCarriedCrystal();

	PlayerController->UnPossess();
	Character->Destroy();
	GameMode->RestartPlayer(PlayerController);
}
