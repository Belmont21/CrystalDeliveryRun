#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestCPP/CDRQuestInteractable.h"
#include "CDRQuestGiver.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UCDRQuestDefinition;

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRQuestGiver : public AActor, public ICDRQuestInteractable
{
	GENERATED_BODY()

public:
	ACDRQuestGiver();

	virtual void Interact_Implementation(AActor* Interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TObjectPtr<UCDRQuestDefinition> QuestDefinition;

private:
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TObjectPtr<UBoxComponent> InteractionBox;

	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TObjectPtr<UStaticMeshComponent> GiverMesh;
};