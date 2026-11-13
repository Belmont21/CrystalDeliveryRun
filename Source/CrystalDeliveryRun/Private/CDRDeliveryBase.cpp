#include "CDRDeliveryBase.h"

#include "CrystalDeliveryRunCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "QuestCPP/CDRQuestGameState.h"
#include "UObject/ConstructorHelpers.h"
#include "Multiplayer/CDRPlayerState.h"
#include "Multiplayer/CDRMatchGameState.h"
#include "Components/PointLightComponent.h"

ACDRDeliveryBase::ACDRDeliveryBase()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

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
    DeliveryLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("DeliveryLight"));
    DeliveryLight->SetupAttachment(RootComponent);
    DeliveryLight->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
    DeliveryLight->SetIntensity(5000.0f);
    DeliveryLight->SetAttenuationRadius(700.0f);
    DeliveryLight->SetLightColor(FLinearColor(0.1f, 1.0f, 0.2f));
    DeliveryLight->SetVisibility(false);


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
    if (!HasAuthority())
    {
        return;
    }

    ACDRMatchGameState* MatchGameState =
   GetWorld()->GetGameState<ACDRMatchGameState>();

    if (IsValid(MatchGameState) &&
        MatchGameState->MatchPhase != ECDRMatchPhase::Playing)
    {
        return;
    }

    
    ACrystalDeliveryRunCharacter* PlayerCharacter =
       Cast<ACrystalDeliveryRunCharacter>(OtherActor);

    if (IsValid(PlayerCharacter) && PlayerCharacter->IsCarryingCrystal())
    {
        PlayerCharacter->DeliverCrystal();
        MulticastDeliveryEffect();
        OnCrystalDelivered();

        ACDRQuestGameState* QuestGameState =
           GetWorld()->GetGameState<ACDRQuestGameState>();

        if (IsValid(QuestGameState))
        {
            QuestGameState->ReportGameplayObjective(
               ECDRQuestObjectiveType::Collect,
               TEXT("CrystalDelivered"),
               1);
        }
    }
}
void ACDRDeliveryBase::MulticastDeliveryEffect_Implementation()
{
    if (!IsValid(DeliveryLight))
    {
        return;
    }

    DeliveryLight->SetVisibility(true);

    GetWorldTimerManager().SetTimer(
       DeliveryLightTimer,
       this,
       &ACDRDeliveryBase::HideDeliveryLight,
       0.6f,
       false);
}

void ACDRDeliveryBase::HideDeliveryLight()
{
    if (IsValid(DeliveryLight))
    {
        DeliveryLight->SetVisibility(false);
    }
}
