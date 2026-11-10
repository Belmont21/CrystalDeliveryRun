#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDRDamageZone.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRDamageZone : public AActor
{
	GENERATED_BODY()

public:
	ACDRDamageZone();

protected:
	UFUNCTION()
	void HandleDamageOverlap(
	   UPrimitiveComponent* OverlappedComponent,
	   AActor* OtherActor,
	   UPrimitiveComponent* OtherComp,
	   int32 OtherBodyIndex,
	   bool bFromSweep,
	   const FHitResult& SweepResult);

	UPROPERTY(EditInstanceOnly, Category = "Damage", meta = (ClampMin = "0.0"))
	float DamageAmount = 25.0f;

private:
	UPROPERTY(VisibleAnywhere, Category = "Damage")
	TObjectPtr<UBoxComponent> DamageBox;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	TObjectPtr<UStaticMeshComponent> DamageMesh;
};
