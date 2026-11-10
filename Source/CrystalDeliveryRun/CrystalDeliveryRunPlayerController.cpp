// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrystalDeliveryRunPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "CDRGameHUDWidget.h"
#include "CrystalDeliveryRun.h"
#include "CrystalDeliveryRunCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Multiplayer/CDRMatchGameState.h"
#include "Multiplayer/CDRPlayerState.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "Multiplayer/CDRMatchGameMode.h"

void ACrystalDeliveryRunPlayerController::BeginPlay()
{
   Super::BeginPlay();

   if (IsLocalPlayerController())
   {
      FInputModeGameOnly InputMode;
      SetInputMode(InputMode);
      bShowMouseCursor = false;

      ResetIgnoreMoveInput();
      ResetIgnoreLookInput();
   }

   InitializeMatchHUD();

   
   if (ShouldUseTouchControls() && IsLocalPlayerController())
   {
      MobileControlsWidget = CreateWidget<UUserWidget>(
         this,
         MobileControlsWidgetClass);

      if (MobileControlsWidget)
      {
         MobileControlsWidget->AddToPlayerScreen(0);
      }
      else
      {
         UE_LOG(
            LogCrystalDeliveryRun,
            Error,
            TEXT("Could not spawn mobile controls widget."));
      }
   }
}

void ACrystalDeliveryRunPlayerController::SetupInputComponent()
{
   Super::SetupInputComponent();

   if (IsLocalPlayerController())
   {
      if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
         ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
            GetLocalPlayer()))
      {
         for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
         {
            Subsystem->AddMappingContext(CurrentContext, 0);
         }

         if (!ShouldUseTouchControls())
         {
            for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
            {
               Subsystem->AddMappingContext(CurrentContext, 0);
            }
         }
      }
   }
}

bool ACrystalDeliveryRunPlayerController::ShouldUseTouchControls() const
{
   return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void ACrystalDeliveryRunPlayerController::OnRep_PlayerState()
{
   Super::OnRep_PlayerState();

   InitializeMatchHUD();
}

void ACrystalDeliveryRunPlayerController::InitializeMatchHUD()
{
   if (!IsLocalPlayerController())
   {
      return;
   }

   ACDRPlayerState* CDRPlayerState = GetPlayerState<ACDRPlayerState>();

   if (!IsValid(CDRPlayerState))
   {
      return;
   }

   if (!IsValid(MatchHUDWidget))
   {
      MatchHUDWidget = CreateWidget<UCDRGameHUDWidget>(this);

      if (!IsValid(MatchHUDWidget))
      {
         return;
      }

      MatchHUDWidget->AddToViewport();
   }

   if (BoundPlayerState.Get() != CDRPlayerState)
   {
      if (BoundPlayerState.IsValid())
      {
         BoundPlayerState->OnCrystalScoreChanged.RemoveAll(this);
      }

      BoundPlayerState = CDRPlayerState;

      CDRPlayerState->OnCrystalScoreChanged.AddUObject(
         this,
         &ACrystalDeliveryRunPlayerController::UpdateMatchScore);
   }

   UpdateMatchScore(CDRPlayerState->CrystalScore);
   BindMatchTimer();
   BindCharacterHealth();
}

void ACrystalDeliveryRunPlayerController::UpdateMatchScore(int32 NewScore)
{
   if (IsValid(MatchHUDWidget))
   {
      MatchHUDWidget->SetScore(NewScore);
   }
}

void ACrystalDeliveryRunPlayerController::BindMatchTimer()
{
   if (!IsLocalPlayerController() || !IsValid(MatchHUDWidget))
   {
      return;
   }

   ACDRMatchGameState* MatchGameState =
      GetWorld()->GetGameState<ACDRMatchGameState>();

   if (!IsValid(MatchGameState))
   {
      if (!GetWorldTimerManager().IsTimerActive(BindMatchTimerHandle))
      {
         GetWorldTimerManager().SetTimer(
            BindMatchTimerHandle,
            this,
            &ACrystalDeliveryRunPlayerController::BindMatchTimer,
            0.2f,
            true);
      }

      return;
   }

   GetWorldTimerManager().ClearTimer(BindMatchTimerHandle);

   if (BoundMatchGameState.Get() != MatchGameState)
   {
      if (BoundMatchGameState.IsValid())
      {
         BoundMatchGameState->OnRemainingTimeChanged.RemoveAll(this);
         BoundMatchGameState->OnTotalScoreChanged.RemoveAll(this);
         BoundMatchGameState->OnMatchPhaseChanged.RemoveAll(this);
      }

      BoundMatchGameState = MatchGameState;

      MatchGameState->OnRemainingTimeChanged.AddUObject(
         this,
         &ACrystalDeliveryRunPlayerController::UpdateMatchTimer);
      MatchGameState->OnTotalScoreChanged.AddUObject(
   this,
   &ACrystalDeliveryRunPlayerController::UpdateTotalScore);
      
      MatchGameState->OnMatchPhaseChanged.AddUObject(
         this,
         &ACrystalDeliveryRunPlayerController::UpdateMatchPhase);


   }

   UpdateMatchTimer(MatchGameState->RemainingTime);
   UpdateTotalScore(MatchGameState->TotalScore);
   UpdateMatchPhase(MatchGameState->MatchPhase);
}

void ACrystalDeliveryRunPlayerController::UpdateMatchTimer(int32 Seconds)
{
   if (IsValid(MatchHUDWidget))
   {
      MatchHUDWidget->SetRemainingTime(Seconds);
   }
}

void ACrystalDeliveryRunPlayerController::OnPossess(APawn* InPawn)
{
   Super::OnPossess(InPawn);

   BindCharacterHealth();
}

void ACrystalDeliveryRunPlayerController::OnRep_Pawn()
{
   Super::OnRep_Pawn();

   BindCharacterHealth();
}

void ACrystalDeliveryRunPlayerController::BindCharacterHealth()
{
   if (!IsLocalPlayerController() || !IsValid(MatchHUDWidget))
   {
      return;
   }

   ACrystalDeliveryRunCharacter* CDRCharacter =
      Cast<ACrystalDeliveryRunCharacter>(GetPawn());

   if (BoundHealthCharacter.Get() != CDRCharacter)
   {
      if (BoundHealthCharacter.IsValid())
      {
         BoundHealthCharacter->OnHealthChanged.RemoveAll(this);
      }

      BoundHealthCharacter = CDRCharacter;

      if (IsValid(CDRCharacter))
      {
         CDRCharacter->OnHealthChanged.AddUObject(
            this,
            &ACrystalDeliveryRunPlayerController::UpdateCharacterHealth);
      }
   }

   if (IsValid(CDRCharacter))
   {
      UpdateCharacterHealth(CDRCharacter->GetHealth());
   }
}

void ACrystalDeliveryRunPlayerController::UpdateCharacterHealth(float NewHealth)
{
   if (IsValid(MatchHUDWidget))
   {
      MatchHUDWidget->SetHealth(NewHealth);
   }
}

void ACrystalDeliveryRunPlayerController::EndPlay(
   const EEndPlayReason::Type EndPlayReason)
{
   GetWorldTimerManager().ClearTimer(BindMatchTimerHandle);

   if (BoundMatchGameState.IsValid())
   {
      BoundMatchGameState->OnRemainingTimeChanged.RemoveAll(this);
      BoundMatchGameState->OnTotalScoreChanged.RemoveAll(this);
      BoundMatchGameState->OnMatchPhaseChanged.RemoveAll(this);
   }

   if (BoundPlayerState.IsValid())
   {
      BoundPlayerState->OnCrystalScoreChanged.RemoveAll(this);
   }

   if (BoundHealthCharacter.IsValid())
   {
      BoundHealthCharacter->OnHealthChanged.RemoveAll(this);
   }

   Super::EndPlay(EndPlayReason);
}

void ACrystalDeliveryRunPlayerController::ClientEndRound_Implementation(
   const FString& Result)
{
   SetIgnoreMoveInput(true);
   SetIgnoreLookInput(true);

   if (Result.StartsWith(TEXT("VICTORIA\n\n")) && IsValid(VictorySound))
   {
      UGameplayStatics::PlaySound2D(this, VictorySound);
   }
   else if (Result.StartsWith(TEXT("DERROTA\n\n")) && IsValid(DefeatSound))
   {
      UGameplayStatics::PlaySound2D(this, DefeatSound);
   }

   if (IsValid(MatchHUDWidget))
   {
      MatchHUDWidget->ShowMatchResult(FText::FromString(Result));

      FInputModeGameAndUI InputMode;
      InputMode.SetWidgetToFocus(MatchHUDWidget->TakeWidget());
      InputMode.SetHideCursorDuringCapture(false);

      SetInputMode(InputMode);
      bShowMouseCursor = true;
   }
}


void ACrystalDeliveryRunPlayerController::ServerRequestRestart_Implementation()
{
   ACDRMatchGameState* MatchGameState =
      GetWorld()->GetGameState<ACDRMatchGameState>();

   if (!IsValid(MatchGameState) ||
       MatchGameState->MatchPhase != ECDRMatchPhase::Finished)
   {
      return;
   }

   const FString LevelName = UGameplayStatics::GetCurrentLevelName(this, true);

   GetWorld()->ServerTravel(LevelName + TEXT("?listen"));
}

void ACrystalDeliveryRunPlayerController::ClientCrystalLost_Implementation()
{
   if (IsValid(MatchHUDWidget))
   {
      MatchHUDWidget->ShowCrystalLost();
   }
}

void ACrystalDeliveryRunPlayerController::ClientCrystalDelivered_Implementation(
   int32 Points)
{
   if (IsValid(CrystalDeliverySound))
   {
      UGameplayStatics::PlaySound2D(this, CrystalDeliverySound);
   }

   if (IsValid(MatchHUDWidget))
   {
      MatchHUDWidget->ShowCrystalDelivered(Points);
   }
}


void ACrystalDeliveryRunPlayerController::UpdateTotalScore(int32 NewTotalScore)
{
   if (IsValid(MatchHUDWidget))
   {
      MatchHUDWidget->SetTotalScore(NewTotalScore);
   }
}
void ACrystalDeliveryRunPlayerController::ServerRequestStartRound_Implementation()
{
   
   if (!IsLocalController())
   {
      return;
   }

   ACDRMatchGameMode* MatchGameMode =
      GetWorld()->GetAuthGameMode<ACDRMatchGameMode>();

   if (IsValid(MatchGameMode))
   {
      MatchGameMode->StartRound();
   }
}
void ACrystalDeliveryRunPlayerController::UpdateMatchPhase(
   ECDRMatchPhase NewPhase)
{
   if (!IsLocalPlayerController() || !IsValid(MatchHUDWidget))
   {
      return;
   }

   const bool bIsLobby = NewPhase == ECDRMatchPhase::Lobby;
   const bool bIsHost = HasAuthority();
   
   if (bWasInLobby &&
    NewPhase == ECDRMatchPhase::Playing &&
    IsValid(GameStartSound))
   {
      UGameplayStatics::PlaySound2D(this, GameStartSound);
   }

   bWasInLobby = bIsLobby;


   MatchHUDWidget->SetLobbyState(bIsLobby, bIsHost);

   if (bIsLobby && bIsHost)
   {
      FInputModeGameAndUI InputMode;
      InputMode.SetWidgetToFocus(MatchHUDWidget->TakeWidget());
      InputMode.SetHideCursorDuringCapture(false);

      SetInputMode(InputMode);
      bShowMouseCursor = true;
   }
   else if (NewPhase == ECDRMatchPhase::Playing)
   {
      FInputModeGameOnly InputMode;
      SetInputMode(InputMode);
      bShowMouseCursor = false;
   }
}
void ACrystalDeliveryRunPlayerController::ClientPlayHurtSound_Implementation()
{
   if (IsValid(HurtSound))
   {
      UGameplayStatics::PlaySound2D(this, HurtSound);
   }
}
void ACrystalDeliveryRunPlayerController::ClientPlayCrystalPickupSound_Implementation()
{
   if (IsValid(CrystalPickupSound))
   {
      UGameplayStatics::PlaySound2D(this, CrystalPickupSound);
   }
}
void ACrystalDeliveryRunPlayerController::ClientPlayFallSound_Implementation()
{
   if (IsValid(FallSound))
   {
      UGameplayStatics::PlaySound2D(this, FallSound);
   }
}


