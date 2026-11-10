// Copyright Epic Games, Inc. All Rights Reserved.

#include "CrystalDeliveryRunCharacter.h"

#include "Camera/CameraComponent.h"
#include "CDRCrystal.h"
#include "CDRGameHUDWidget.h"
#include "Components/CapsuleComponent.h"
#include "CrystalDeliveryRun.h"
#include "CrystalDeliveryRunPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Multiplayer/CDRMatchGameState.h"
#include "Multiplayer/CDRPlayerState.h"
#include "Net/UnrealNetwork.h"

ACrystalDeliveryRunCharacter::ACrystalDeliveryRunCharacter()
{
   GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

   bUseControllerRotationPitch = false;
   bUseControllerRotationYaw = false;
   bUseControllerRotationRoll = false;

   GetCharacterMovement()->bOrientRotationToMovement = true;
   GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

   GetCharacterMovement()->JumpZVelocity = 500.0f;
   GetCharacterMovement()->AirControl = 0.35f;
   GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
   GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
   GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
   GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

   CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
   CameraBoom->SetupAttachment(RootComponent);
   CameraBoom->TargetArmLength = 400.0f;
   CameraBoom->bUsePawnControlRotation = true;

   FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
   FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
   FollowCamera->bUsePawnControlRotation = false;
}

void ACrystalDeliveryRunCharacter::BeginPlay()
{
   Super::BeginPlay();

   if (!bCreateLegacyHUD)
   {
      return;
   }

   APlayerController* PlayerController = Cast<APlayerController>(GetController());

   if (!IsValid(PlayerController))
   {
      return;
   }

   GameHUDWidget = CreateWidget<UCDRGameHUDWidget>(PlayerController);

   if (IsValid(GameHUDWidget))
   {
      GameHUDWidget->AddToViewport();
   }
}

void ACrystalDeliveryRunCharacter::SetupPlayerInputComponent(
   UInputComponent* PlayerInputComponent)
{
   if (UEnhancedInputComponent* EnhancedInputComponent =
      Cast<UEnhancedInputComponent>(PlayerInputComponent))
   {
      
      EnhancedInputComponent->BindAction(
         JumpAction,
         ETriggerEvent::Started,
         this,
         &ACharacter::Jump);

      EnhancedInputComponent->BindAction(
         JumpAction,
         ETriggerEvent::Completed,
         this,
         &ACharacter::StopJumping);

      
      EnhancedInputComponent->BindAction(
         MoveAction,
         ETriggerEvent::Triggered,
         this,
         &ACrystalDeliveryRunCharacter::Move);

      EnhancedInputComponent->BindAction(
         MouseLookAction,
         ETriggerEvent::Triggered,
         this,
         &ACrystalDeliveryRunCharacter::Look);

      EnhancedInputComponent->BindAction(
         LookAction,
         ETriggerEvent::Triggered,
         this,
         &ACrystalDeliveryRunCharacter::Look);

      // Correr con SHIIFT
      if (IsValid(SprintAction))
      {
         EnhancedInputComponent->BindAction(
            SprintAction,
            ETriggerEvent::Started,
            this,
            &ACrystalDeliveryRunCharacter::StartSprint);

         EnhancedInputComponent->BindAction(
            SprintAction,
            ETriggerEvent::Completed,
            this,
            &ACrystalDeliveryRunCharacter::StopSprint);

         EnhancedInputComponent->BindAction(
            SprintAction,
            ETriggerEvent::Canceled,
            this,
            &ACrystalDeliveryRunCharacter::StopSprint);
      }
   }
   else
   {
      UE_LOG(
         LogCrystalDeliveryRun,
         Error,
         TEXT("'%s' Failed to find an Enhanced Input component!"),
         *GetNameSafe(this));
   }
}

void ACrystalDeliveryRunCharacter::Move(const FInputActionValue& Value)
{
   FVector2D MovementVector = Value.Get<FVector2D>();

   DoMove(MovementVector.X, MovementVector.Y);
}

void ACrystalDeliveryRunCharacter::Look(const FInputActionValue& Value)
{
   FVector2D LookAxisVector = Value.Get<FVector2D>();

   DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ACrystalDeliveryRunCharacter::DoMove(float Right, float Forward)
{
   if (GetController() != nullptr)
   {
      const FRotator Rotation = GetController()->GetControlRotation();
      const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

      const FVector ForwardDirection =
         FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

      const FVector RightDirection =
         FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

      AddMovementInput(ForwardDirection, Forward);
      AddMovementInput(RightDirection, Right);
   }
}

void ACrystalDeliveryRunCharacter::DoLook(float Yaw, float Pitch)
{
   if (GetController() != nullptr)
   {
      AddControllerYawInput(Yaw);
      AddControllerPitchInput(Pitch);
   }
}

void ACrystalDeliveryRunCharacter::DoJumpStart()
{
   Jump();
}

void ACrystalDeliveryRunCharacter::DoJumpEnd()
{
   StopJumping();
}

void ACrystalDeliveryRunCharacter::StartSprint()
{
   GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

   if (!HasAuthority())
   {
      ServerSetSprinting(true);
   }
}

void ACrystalDeliveryRunCharacter::StopSprint()
{
   GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

   if (!HasAuthority())
   {
      ServerSetSprinting(false);
   }
}

void ACrystalDeliveryRunCharacter::ServerSetSprinting_Implementation(
   bool bWantsToSprint)
{
   GetCharacterMovement()->MaxWalkSpeed =
      bWantsToSprint ? SprintSpeed : WalkSpeed;
}

bool ACrystalDeliveryRunCharacter::TryPickUpCrystal(ACDRCrystal* Crystal)
{
   if (!IsValid(Crystal) || IsCarryingCrystal())
   {
      return false;
   }

   CarriedCrystal = Crystal;

   Crystal->SetActorEnableCollision(false);
   Crystal->AttachToComponent(
      GetRootComponent(),
      FAttachmentTransformRules::SnapToTargetNotIncludingScale);

   Crystal->SetActorRelativeLocation(FVector(100.0f, 0.0f, 80.0f));
   if (HasAuthority())
   {
      ACrystalDeliveryRunPlayerController* CDRPlayerController =
         Cast<ACrystalDeliveryRunPlayerController>(GetController());

      if (IsValid(CDRPlayerController))
      {
         CDRPlayerController->ClientPlayCrystalPickupSound();
      }
   }


   return true;
}

bool ACrystalDeliveryRunCharacter::IsCarryingCrystal() const
{
   return IsValid(CarriedCrystal);
}

void ACrystalDeliveryRunCharacter::DeliverCrystal()
{
   if (!IsCarryingCrystal())
   {
      return;
   }

   if (ACDRPlayerState* CDRPlayerState = GetPlayerState<ACDRPlayerState>())
   {
      CDRPlayerState->RegisterCrystalDelivery(CarriedCrystal);
   }

   Score += CarriedCrystal->GetPointValue();

   if (IsValid(GameHUDWidget))
   {
      GameHUDWidget->SetScore(Score);
   }

   ACrystalDeliveryRunPlayerController* CDRPlayerController =
      Cast<ACrystalDeliveryRunPlayerController>(GetController());

   if (IsValid(CDRPlayerController))
   {
      CDRPlayerController->ClientCrystalDelivered(
         CarriedCrystal->GetPointValue());
   }

   CarriedCrystal->Destroy();
   CarriedCrystal = nullptr;
}

int32 ACrystalDeliveryRunCharacter::GetScore() const
{
   return Score;
}

void ACrystalDeliveryRunCharacter::GetLifetimeReplicatedProps(
   TArray<FLifetimeProperty>& OutLifetimeProps) const
{
   Super::GetLifetimeReplicatedProps(OutLifetimeProps);

   DOREPLIFETIME(ACrystalDeliveryRunCharacter, CarriedCrystal);
   DOREPLIFETIME(ACrystalDeliveryRunCharacter, Health);
}

void ACrystalDeliveryRunCharacter::LoseCarriedCrystal()
{
   if (!HasAuthority() || !IsValid(CarriedCrystal))
   {
      return;
   }

   CarriedCrystal->Destroy();
   CarriedCrystal = nullptr;
}

void ACrystalDeliveryRunCharacter::OnRep_Health()
{
   OnHealthChanged.Broadcast(Health);
}

float ACrystalDeliveryRunCharacter::GetHealth() const
{
   return Health;
}

float ACrystalDeliveryRunCharacter::TakeDamage(
   float DamageAmount,
   const FDamageEvent& DamageEvent,
   AController* EventInstigator,
   AActor* DamageCauser)
{
   if (!HasAuthority() || DamageAmount <= 0.0f || Health <= 0.0f)
   {
      return 0.0f;
   }

   ACDRMatchGameState* MatchGameState =
      GetWorld()->GetGameState<ACDRMatchGameState>();

   if (!IsValid(MatchGameState) ||
       MatchGameState->MatchPhase != ECDRMatchPhase::Playing)
   {
      return 0.0f;
   }

   const float AppliedDamage = Super::TakeDamage(
      DamageAmount,
      DamageEvent,
      EventInstigator,
      DamageCauser);

   if (AppliedDamage <= 0.0f)
   {
      return 0.0f;
   }

   Health = FMath::Clamp(Health - AppliedDamage, 0.0f, 100.0f);
   OnHealthChanged.Broadcast(Health);

   ACrystalDeliveryRunPlayerController* HurtController =
      Cast<ACrystalDeliveryRunPlayerController>(GetController());

   if (IsValid(HurtController))
   {
      HurtController->ClientPlayHurtSound();
   }

   if (Health <= 0.0f)
   {
      ACrystalDeliveryRunPlayerController* CDRPlayerController =
         Cast<ACrystalDeliveryRunPlayerController>(GetController());

      AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();

      if (IsValid(CDRPlayerController) && IsValid(GameMode))
      {
         if (ACDRPlayerState* CDRPlayerState =
            CDRPlayerController->GetPlayerState<ACDRPlayerState>())
         {
            CDRPlayerState->DeathCount++;
         }

         if (IsCarryingCrystal())
         {
            CDRPlayerController->ClientCrystalLost();
            LoseCarriedCrystal();
         }

         CDRPlayerController->UnPossess();
         Destroy();
         GameMode->RestartPlayer(CDRPlayerController);
      }
   }

   return AppliedDamage;
}

