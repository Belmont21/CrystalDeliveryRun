#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDRDeliveryBase.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRDeliveryBase : public AActor
{
	GENERATED_BODY()

public:
	ACDRDeliveryBase();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Quest")
	void OnCrystalDelivered();
protected:
	UFUNCTION()
	void HandleBaseOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Base")
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere, Category = "Base")
	TObjectPtr<UStaticMeshComponent> BaseMesh;
};
