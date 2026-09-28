#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDRMainMenuWidget.generated.h"

class UButton;

UCLASS()
class CRYSTALDELIVERYRUN_API UCDRMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UButton> HostButton;

	UPROPERTY(Transient)
	TObjectPtr<UButton> QuitButton;

	UFUNCTION()
	void HandleHostClicked();

	UFUNCTION()
	void HandleQuitClicked();
};
