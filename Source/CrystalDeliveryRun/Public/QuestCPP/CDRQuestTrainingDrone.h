#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestCPP/CDRQuestInteractable.h"
#include "CDRQuestTrainingDrone.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRQuestTrainingDrone : public AActor, public ICDRQuestInteractable
{
	GENERATED_BODY()

public:
	ACDRQuestTrainingDrone();

	virtual void Interact_Implementation(AActor* Interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName TargetID = TEXT("CPPTrainingDrone");

private:
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TObjectPtr<UBoxComponent> InteractionBox;

	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TObjectPtr<UStaticMeshComponent> DroneMesh;
};
