#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDRHazardZone.generated.h"

class UBoxComponent;
class UPrimitiveComponent;

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRHazardZone : public AActor
{
	GENERATED_BODY()

public:
	ACDRHazardZone();

private:
	UFUNCTION()
	void HandleHazardOverlap(
	   UPrimitiveComponent* OverlappedComponent,
	   AActor* OtherActor,
	   UPrimitiveComponent* OtherComp,
	   int32 OtherBodyIndex,
	   bool bFromSweep,
	   const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, Category = "Hazard")
	TObjectPtr<UBoxComponent> HazardBox;
};
