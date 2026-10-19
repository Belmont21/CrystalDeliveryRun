#include "QuestCPP/CDRQuestReachLocation.h"

#include "CrystalDeliveryRunCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "QuestCPP/CDRQuestGameState.h"
#include "UObject/ConstructorHelpers.h"

ACDRQuestReachLocation::ACDRQuestReachLocation()
{
    PrimaryActorTick.bCanEverTick = false;

    ReachBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ReachBox"));
    RootComponent = ReachBox;

    ReachBox->SetBoxExtent(FVector(180.0f, 180.0f, 150.0f));
    ReachBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ReachBox->SetCollisionObjectType(ECC_WorldDynamic);
    ReachBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    ReachBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    ReachBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &ACDRQuestReachLocation::HandleReachOverlap);

    BeaconMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("BeaconMesh"));

    BeaconMesh->SetupAttachment(RootComponent);
    BeaconMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BeaconMesh->SetRelativeScale3D(FVector(1.2f, 1.2f, 3.0f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(
        TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));

    if (CylinderMesh.Succeeded())
    {
        BeaconMesh->SetStaticMesh(CylinderMesh.Object);
    }
}

void ACDRQuestReachLocation::HandleReachOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!Cast<ACrystalDeliveryRunCharacter>(OtherActor))
    {
        return;
    }

    ACDRQuestGameState* QuestGameState =
        GetWorld()->GetGameState<ACDRQuestGameState>();

    if (IsValid(QuestGameState))
    {
        QuestGameState->ReportGameplayObjective(
            ECDRQuestObjectiveType::ReachLocation,
            TargetID,
            1);
    }
}
