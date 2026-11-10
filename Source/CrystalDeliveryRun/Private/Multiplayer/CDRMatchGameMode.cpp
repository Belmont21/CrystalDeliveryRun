#include "Multiplayer/CDRMatchGameMode.h"

#include "CrystalDeliveryRunPlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Multiplayer/CDRMatchGameState.h"
#include "Multiplayer/CDRPlayerState.h"

ACDRMatchGameMode::ACDRMatchGameMode()
{
   GameStateClass = ACDRMatchGameState::StaticClass();
   PlayerStateClass = ACDRPlayerState::StaticClass();
}

void ACDRMatchGameMode::BeginPlay()
{
   Super::BeginPlay();

   ACDRMatchGameState* MatchGameState =
      GetGameState<ACDRMatchGameState>();

   if (!IsValid(MatchGameState))
   {
      return;
   }

   // como host hasta que no aprete iniciar no se debe poder correr el tiempo
   MatchGameState->MatchPhase = ECDRMatchPhase::Lobby;
   MatchGameState->OnMatchPhaseChanged.Broadcast(MatchGameState->MatchPhase);
   MatchGameState->RemainingTime = RoundDuration;
}

void ACDRMatchGameMode::StartRound()
{
   ACDRMatchGameState* MatchGameState =
      GetGameState<ACDRMatchGameState>();

   if (!IsValid(MatchGameState) ||
       MatchGameState->MatchPhase != ECDRMatchPhase::Lobby)
   {
      return;
   }

   MatchGameState->MatchPhase = ECDRMatchPhase::Playing;
   MatchGameState->OnMatchPhaseChanged.Broadcast(MatchGameState->MatchPhase);
   MatchGameState->RemainingTime = RoundDuration;

   GetWorldTimerManager().SetTimer(
      RoundTimerHandle,
      this,
      &ACDRMatchGameMode::UpdateRoundTimer,
      1.0f,
      true);
}

void ACDRMatchGameMode::UpdateRoundTimer()
{
   ACDRMatchGameState* MatchGameState =
      GetGameState<ACDRMatchGameState>();

   if (!IsValid(MatchGameState))
   {
      GetWorldTimerManager().ClearTimer(RoundTimerHandle);
      return;
   }

   MatchGameState->RemainingTime--;

   MatchGameState->OnRemainingTimeChanged.Broadcast(
      MatchGameState->RemainingTime);

   if (MatchGameState->RemainingTime <= 0)
   {
      MatchGameState->RemainingTime = 0;

      CalculateWinners();

      FString ResultMessage;

      if (MatchGameState->Winners.Num() == 1)
      {
         ResultMessage = TEXT("GANADOR\n");
      }
      else if (MatchGameState->Winners.Num() > 1)
      {
         ResultMessage = TEXT("EMPATE\n");
      }
      else
      {
         ResultMessage = TEXT("SIN GANADOR\n");
      }

      for (const ACDRPlayerState* Winner : MatchGameState->Winners)
      {
         if (IsValid(Winner))
         {
            ResultMessage += Winner->GetPlayerName() + TEXT("\n");
         }
      }

      if (!MatchGameState->Winners.IsEmpty() &&
          IsValid(MatchGameState->Winners[0]))
      {
         const ACDRPlayerState* Winner = MatchGameState->Winners[0];

         bool bDifferentLegendary = false;
         bool bDifferentRare = false;
         bool bDifferentCommon = false;

         for (APlayerState* PlayerState : MatchGameState->PlayerArray)
         {
            const ACDRPlayerState* Other =
               Cast<ACDRPlayerState>(PlayerState);

            if (!IsValid(Other) ||
                Other == Winner ||
                Other->CrystalScore != Winner->CrystalScore)
            {
               continue;
            }

            if (Other->LegendaryDelivered != Winner->LegendaryDelivered)
            {
               bDifferentLegendary = true;
            }
            else if (Other->RareDelivered != Winner->RareDelivered)
            {
               bDifferentRare = true;
            }
            else if (Other->CommonDelivered != Winner->CommonDelivered)
            {
               bDifferentCommon = true;
            }
         }

         if (bDifferentLegendary)
         {
            ResultMessage += TEXT("\nDesempate: cristales legendarios\n");
         }
         else if (bDifferentRare)
         {
            ResultMessage += TEXT("\nDesempate: cristales raros\n");
         }
         else if (bDifferentCommon)
         {
            ResultMessage += TEXT("\nDesempate: cristales comunes\n");
         }
      }

      ResultMessage += TEXT("\nPUNTAJES\n");

      for (APlayerState* PlayerState : MatchGameState->PlayerArray)
      {
         const ACDRPlayerState* CDRPlayerState =
            Cast<ACDRPlayerState>(PlayerState);

         if (IsValid(CDRPlayerState))
         {
            ResultMessage += FString::Printf(
               TEXT("%s: %d puntos | Muertes: %d\n"),
               *CDRPlayerState->GetPlayerName(),
               CDRPlayerState->CrystalScore,
               CDRPlayerState->DeathCount);
         }
      }

      MatchGameState->MatchPhase = ECDRMatchPhase::Finished;
      MatchGameState->OnMatchPhaseChanged.Broadcast(MatchGameState->MatchPhase);

      for (FConstPlayerControllerIterator It =
         GetWorld()->GetPlayerControllerIterator(); It; ++It)
      {
         ACrystalDeliveryRunPlayerController* PlayerController =
            Cast<ACrystalDeliveryRunPlayerController>(It->Get());

         if (!IsValid(PlayerController))
         {
            continue;
         }

         ACharacter* Character = PlayerController->GetCharacter();

         if (IsValid(Character))
         {
            Character->GetCharacterMovement()->DisableMovement();
         }

         ACDRPlayerState* CurrentPlayerState =
            PlayerController->GetPlayerState<ACDRPlayerState>();

         bool bIsWinner = false;

         for (const ACDRPlayerState* Winner : MatchGameState->Winners)
         {
            if (Winner == CurrentPlayerState && IsValid(Winner))
            {
               bIsWinner = true;
               break;
            }
         }

         FString PersonalResult;

         if (MatchGameState->Winners.IsEmpty())
         {
            PersonalResult = TEXT("PARTIDA FINALIZADA\n\n");
         }
         else if (!bIsWinner)
         {
            PersonalResult = TEXT("DERROTA\n\n");
         }
         else if (MatchGameState->Winners.Num() > 1)
         {
            PersonalResult = TEXT("EMPATE\n\n");
         }
         else
         {
            PersonalResult = TEXT("VICTORIA\n\n");
         }

         PlayerController->ClientEndRound(PersonalResult + ResultMessage);
      }

      GetWorldTimerManager().ClearTimer(RoundTimerHandle);
   }
}

void ACDRMatchGameMode::CalculateWinners()
{
   ACDRMatchGameState* MatchGameState =
      GetGameState<ACDRMatchGameState>();

   if (!IsValid(MatchGameState))
   {
      return;
   }

   MatchGameState->Winners.Empty();

   for (APlayerState* PlayerState : MatchGameState->PlayerArray)
   {
      ACDRPlayerState* Candidate = Cast<ACDRPlayerState>(PlayerState);

      if (!IsValid(Candidate))
      {
         continue;
      }

      if (MatchGameState->Winners.IsEmpty())
      {
         MatchGameState->Winners.Add(Candidate);
         continue;
      }

      const ACDRPlayerState* Best = MatchGameState->Winners[0];

      const bool bCandidateWins =
         Candidate->CrystalScore > Best->CrystalScore ||
         (Candidate->CrystalScore == Best->CrystalScore &&
          Candidate->LegendaryDelivered > Best->LegendaryDelivered) ||
         (Candidate->CrystalScore == Best->CrystalScore &&
          Candidate->LegendaryDelivered == Best->LegendaryDelivered &&
          Candidate->RareDelivered > Best->RareDelivered) ||
         (Candidate->CrystalScore == Best->CrystalScore &&
          Candidate->LegendaryDelivered == Best->LegendaryDelivered &&
          Candidate->RareDelivered == Best->RareDelivered &&
          Candidate->CommonDelivered > Best->CommonDelivered);

      const bool bExactTie =
         Candidate->CrystalScore == Best->CrystalScore &&
         Candidate->LegendaryDelivered == Best->LegendaryDelivered &&
         Candidate->RareDelivered == Best->RareDelivered &&
         Candidate->CommonDelivered == Best->CommonDelivered;

      if (bCandidateWins)
      {
         MatchGameState->Winners.Empty();
         MatchGameState->Winners.Add(Candidate);
      }
      else if (bExactTie)
      {
         MatchGameState->Winners.Add(Candidate);
      }
   }
}



