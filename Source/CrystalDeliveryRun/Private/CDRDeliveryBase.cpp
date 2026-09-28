#include "CDRDeliveryBase.h"

#include "CrystalDeliveryRunCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ACDRDeliveryBase::ACDRDeliveryBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetBoxExtent(FVector(250.0f, 250.0f, 150.0f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&ACDRDeliveryBase::HandleBaseOverlap);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseMesh->SetRelativeScale3D(FVector(5.0f, 5.0f, 1.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube"));

	if (CubeMesh.Succeeded())
	{
		BaseMesh->SetStaticMesh(CubeMesh.Object);
	}
}

void ACDRDeliveryBase::HandleBaseOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ACrystalDeliveryRunCharacter* PlayerCharacter =
		Cast<ACrystalDeliveryRunCharacter>(OtherActor);

	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->DeliverCrystal();
	}
}

