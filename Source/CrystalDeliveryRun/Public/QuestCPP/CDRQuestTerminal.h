#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestCPP/CDRQuestInteractable.h"
#include "CDRQuestTerminal.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRQuestTerminal : public AActor, public ICDRQuestInteractable
{
	GENERATED_BODY()

public:
	ACDRQuestTerminal();

	virtual void Interact_Implementation(AActor* Interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName TargetID = TEXT("CPPTerminalAlpha");

private:
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TObjectPtr<UBoxComponent> InteractionBox;

	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TObjectPtr<UStaticMeshComponent> TerminalMesh;
};
