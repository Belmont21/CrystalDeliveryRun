#include "CDRCrystal.h"
#include "CrystalDeliveryRunCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ACDRCrystal::ACDRCrystal()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;

	CollisionSphere->InitSphereRadius(100.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this,&ACDRCrystal::HandleCrystalOverlap);

	CrystalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalMesh"));
	CrystalMesh->SetupAttachment(RootComponent);
	CrystalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CrystalMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(
		TEXT("/Engine/BasicShapes/Cone.Cone"));

	if (ConeMesh.Succeeded())
	{
		CrystalMesh->SetStaticMesh(ConeMesh.Object);
	}
}

int32 ACDRCrystal::GetPointValue() const
{
	switch (Rarity)
	{
	case ECDRCrystalRarity::Common:
		return 1;

	case ECDRCrystalRarity::Rare:
		return 3;

	case ECDRCrystalRarity::Legendary:
		return 5;

	default:
		return 1;
	}
}
	
	void ACDRCrystal::HandleCrystalOverlap(
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
			PlayerCharacter->TryPickUpCrystal(this);
		}
	}
