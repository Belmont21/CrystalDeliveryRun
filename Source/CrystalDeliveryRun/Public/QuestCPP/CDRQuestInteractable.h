#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CDRQuestInteractable.generated.h"

class AActor;

UINTERFACE(BlueprintType)
class CRYSTALDELIVERYRUN_API UCDRQuestInteractable : public UInterface
{
	GENERATED_BODY()
};

class CRYSTALDELIVERYRUN_API ICDRQuestInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quest")
	void Interact(AActor* Interactor);
};