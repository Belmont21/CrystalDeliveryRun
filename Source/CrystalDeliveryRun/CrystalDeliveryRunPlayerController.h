// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "CrystalDeliveryRunPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UCDRGameHUDWidget;
class ACDRPlayerState;
class ACDRMatchGameState;
class ACrystalDeliveryRunCharacter;
enum class ECDRMatchPhase : uint8;
class USoundBase;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class ACrystalDeliveryRunPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Client, Reliable)
	void ClientPlayFallSound();

	UFUNCTION(Client, Reliable)
	void ClientPlayCrystalPickupSound();
	
	UFUNCTION(Client, Reliable)
	void ClientPlayHurtSound();
	
	UFUNCTION(Client, Reliable)
	void ClientEndRound(const FString& Result);
	
	UFUNCTION(Server, Reliable)
	void ServerRequestRestart();
	
	UFUNCTION(Client, Reliable)
	void ClientCrystalLost();
	
	UFUNCTION(Client, Reliable)
	void ClientCrystalDelivered(int32 Points);
	
	UFUNCTION(Client, Reliable)
	void ServerRequestStartRound();
	
protected:
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;

	void BindCharacterHealth();
	void UpdateCharacterHealth(float NewHealth);

	TWeakObjectPtr<ACrystalDeliveryRunCharacter> BoundHealthCharacter;
	
	virtual void OnRep_PlayerState() override;

	void InitializeMatchHUD();
	void UpdateMatchScore(int32 NewScore);
	void UpdateTotalScore(int32 NewTotalScore);
	void BindMatchTimer();
	void UpdateMatchTimer(int32 Seconds);
	void UpdateMatchPhase(ECDRMatchPhase NewPhase);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle BindMatchTimerHandle;
	TWeakObjectPtr<ACDRMatchGameState> BoundMatchGameState;
	
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> CrystalPickupSound;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> HurtSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> VictorySound;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> DefeatSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> GameStartSound;

	bool bWasInLobby = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> FallSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	TObjectPtr<USoundBase> CrystalDeliverySound;
	
	UPROPERTY(Transient)
	TObjectPtr<UCDRGameHUDWidget> MatchHUDWidget;

	TWeakObjectPtr<ACDRPlayerState> BoundPlayerState;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** If true, the player will use UMG touch controls even if not playing on mobile platforms */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	/** Returns true if the player should use UMG touch controls */
	bool ShouldUseTouchControls() const;

};
