
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "CDRMainMenuWidget.generated.h"

class UButton;
class UEditableTextBox;
class USoundBase;
class UTextBlock;
class UTexture2D;

UCLASS()
class CRYSTALDELIVERYRUN_API UCDRMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UCDRMainMenuWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	void PlayMenuClick();
	void QuitGameAfterClick();

	FTimerHandle QuitTimerHandle;
	
	FTimerHandle MenuMessageTimerHandle;
	void ClearMenuMessage();

	UPROPERTY(Transient)
	TObjectPtr<USoundBase> ClickSound;

	UPROPERTY(Transient)
	TObjectPtr<UTexture2D> MenuBackground;

	UPROPERTY(Transient)
	TObjectPtr<UButton> HostButton;

	UPROPERTY(Transient)
	TObjectPtr<UButton> JoinButton;

	UPROPERTY(Transient)
	TObjectPtr<UButton> QuitButton;

	UPROPERTY(Transient)
	TObjectPtr<UEditableTextBox> AddressInput;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> MenuMessage;

	UFUNCTION()
	void HandleHostClicked();

	UFUNCTION()
	void HandleJoinClicked();

	UFUNCTION()
	void HandleQuitClicked();
};

