#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "CDRGameInstance.generated.h"

class UNetDriver;
class UWorld;

UCLASS()
class CRYSTALDELIVERYRUN_API UCDRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	FText ConsumeNetworkError();

private:
	void HandleNetworkFailure(
	   UWorld* World,
	   UNetDriver* NetDriver,
	   ENetworkFailure::Type FailureType,
	   const FString& ErrorString);

	FText PendingNetworkError;
	FDelegateHandle NetworkFailureHandle;
};
