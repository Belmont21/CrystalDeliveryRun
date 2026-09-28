#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CDRMainMenuGameMode.generated.h"

class UCDRMainMenuWidget;

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UCDRMainMenuWidget> MainMenuWidget;
};
