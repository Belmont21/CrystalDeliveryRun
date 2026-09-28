#include "CDRMainMenuGameMode.h"

#include "CDRMainMenuWidget.h"
#include "GameFramework/PlayerController.h"

void ACDRMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!IsValid(PlayerController))
	{
		return;
	}

	MainMenuWidget = CreateWidget<UCDRMainMenuWidget>(PlayerController);

	if (!IsValid(MainMenuWidget))
	{
		return;
	}

	MainMenuWidget->AddToViewport();

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}
