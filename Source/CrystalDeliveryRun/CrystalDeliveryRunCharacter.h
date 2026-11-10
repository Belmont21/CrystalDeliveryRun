// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CrystalDeliveryRunCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class ACDRCrystal;
class UCDRGameHUDWidget;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_MULTICAST_DELEGATE_OneParam(FCDRHealthChanged, float);

UCLASS(abstract)
class ACrystalDeliveryRunCharacter : public ACharacter
{
   GENERATED_BODY()

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
   USpringArmComponent* CameraBoom;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
   UCameraComponent* FollowCamera;

protected:
   UPROPERTY(EditAnywhere, Category = "Input")
   UInputAction* JumpAction;

   UPROPERTY(EditAnywhere, Category = "Input")
   UInputAction* MoveAction;

   UPROPERTY(EditAnywhere, Category = "Input")
   UInputAction* LookAction;

   UPROPERTY(EditAnywhere, Category = "Input")
   UInputAction* MouseLookAction;

   UPROPERTY(EditDefaultsOnly, Category = "Input")
   TObjectPtr<UInputAction> SprintAction;

   UPROPERTY(EditDefaultsOnly, Category = "Movement|Sprint", meta = (ClampMin = "0.0"))
   float WalkSpeed = 500.0f;

   UPROPERTY(EditDefaultsOnly, Category = "Movement|Sprint", meta = (ClampMin = "0.0"))
   float SprintSpeed = 750.0f;

   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
   bool bCreateLegacyHUD = true;

public:
   FCDRHealthChanged OnHealthChanged;

   float GetHealth() const;

   ACrystalDeliveryRunCharacter();

   virtual void GetLifetimeReplicatedProps(
      TArray<FLifetimeProperty>& OutLifetimeProps) const override;

   virtual void BeginPlay() override;

   bool TryPickUpCrystal(ACDRCrystal* Crystal);
   bool IsCarryingCrystal() const;
   void DeliverCrystal();
   void LoseCarriedCrystal();
   int32 GetScore() const;

   virtual float TakeDamage(
      float DamageAmount,
      const FDamageEvent& DamageEvent,
      AController* EventInstigator,
      AActor* DamageCauser) override;

protected:
   virtual void SetupPlayerInputComponent(
      class UInputComponent* PlayerInputComponent) override;

   void Move(const FInputActionValue& Value);
   void Look(const FInputActionValue& Value);

   void StartSprint();
   void StopSprint();

   UFUNCTION(Server, Reliable)
   void ServerSetSprinting(bool bWantsToSprint);

public:
   UFUNCTION(BlueprintCallable, Category = "Input")
   virtual void DoMove(float Right, float Forward);

   UFUNCTION(BlueprintCallable, Category = "Input")
   virtual void DoLook(float Yaw, float Pitch);

   UFUNCTION(BlueprintCallable, Category = "Input")
   virtual void DoJumpStart();

   UFUNCTION(BlueprintCallable, Category = "Input")
   virtual void DoJumpEnd();

   FORCEINLINE class USpringArmComponent* GetCameraBoom() const
   {
      return CameraBoom;
   }

   FORCEINLINE class UCameraComponent* GetFollowCamera() const
   {
      return FollowCamera;
   }

private:
   UPROPERTY(ReplicatedUsing = OnRep_Health)
   float Health = 100.0f;

   UFUNCTION()
   void OnRep_Health();

   UPROPERTY(Replicated)
   TObjectPtr<ACDRCrystal> CarriedCrystal;

   UPROPERTY()
   int32 Score = 0;

   UPROPERTY(Transient)
   TObjectPtr<UCDRGameHUDWidget> GameHUDWidget;
};


