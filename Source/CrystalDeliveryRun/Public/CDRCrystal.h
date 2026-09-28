#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDRCrystal.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

UENUM()
enum class ECDRCrystalRarity : uint8
{
	Common,
	Rare,
	Legendary
};

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRCrystal : public AActor
{
	GENERATED_BODY()

public:
	ACDRCrystal();

	int32 GetPointValue() const;
	
protected:
	UFUNCTION()
	void HandleCrystalOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);
	

private:
	UPROPERTY(VisibleAnywhere, Category = "Crystal")
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Crystal")
	TObjectPtr<UStaticMeshComponent> CrystalMesh;

	UPROPERTY(EditAnywhere, Category = "Crystal")
	ECDRCrystalRarity Rarity = ECDRCrystalRarity::Common;
};
