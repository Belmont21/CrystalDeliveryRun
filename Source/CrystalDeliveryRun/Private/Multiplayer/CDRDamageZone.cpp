#include "Multiplayer/CDRDamageZone.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CrystalDeliveryRunCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACDRDamageZone::ACDRDamageZone()
{
	PrimaryActorTick.bCanEverTick = false;

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	RootComponent = DamageBox;

	DamageBox->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	DamageBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageBox->SetCollisionObjectType(ECC_WorldDynamic);
	DamageBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DamageBox->SetGenerateOverlapEvents(true);

	DamageBox->OnComponentBeginOverlap.AddDynamic(
	   this,
	   &ACDRDamageZone::HandleDamageOverlap);

	DamageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DamageMesh"));
	DamageMesh->SetupAttachment(RootComponent);
	DamageMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
	   TEXT("/Engine/BasicShapes/Cube.Cube"));

	if (CubeMesh.Succeeded())
	{
		DamageMesh->SetStaticMesh(CubeMesh.Object);
	}
}

void ACDRDamageZone::HandleDamageOverlap(
   UPrimitiveComponent* OverlappedComponent,
   AActor* OtherActor,
   UPrimitiveComponent* OtherComp,
   int32 OtherBodyIndex,
   bool bFromSweep,
   const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	ACrystalDeliveryRunCharacter* PlayerCharacter =
	   Cast<ACrystalDeliveryRunCharacter>(OtherActor);

	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	UGameplayStatics::ApplyDamage(
	   PlayerCharacter,
	   DamageAmount,
	   nullptr,
	   this,
	   nullptr);
}

