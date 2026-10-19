#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDRQuestReachLocation.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UPrimitiveComponent;

UCLASS()
class CRYSTALDELIVERYRUN_API ACDRQuestReachLocation : public AActor
{
	GENERATED_BODY()

public:
	ACDRQuestReachLocation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName TargetID = TEXT("CPPExitBeacon");

protected:
	UFUNCTION()
	void HandleReachOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TObjectPtr<UBoxComponent> ReachBox;

	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TObjectPtr<UStaticMeshComponent> BeaconMesh;
};
