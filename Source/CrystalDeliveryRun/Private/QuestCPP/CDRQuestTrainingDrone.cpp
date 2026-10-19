#include "QuestCPP/CDRQuestTrainingDrone.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "QuestCPP/CDRQuestComponent.h"
#include "QuestCPP/CDRQuestGameState.h"
#include "UObject/ConstructorHelpers.h"

ACDRQuestTrainingDrone::ACDRQuestTrainingDrone()
{
    PrimaryActorTick.bCanEverTick = false;

    InteractionBox = CreateDefaultSubobject<UBoxComponent>(
        TEXT("InteractionBox"));

    RootComponent = InteractionBox;

    InteractionBox->SetBoxExtent(FVector(120.0f, 120.0f, 120.0f));
    InteractionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionBox->SetCollisionObjectType(ECC_WorldDynamic);
    InteractionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("DroneMesh"));

    DroneMesh->SetupAttachment(RootComponent);
    DroneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DroneMesh->SetRelativeScale3D(FVector(1.5f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
        TEXT("/Engine/BasicShapes/Sphere.Sphere"));

    if (SphereMesh.Succeeded())
    {
        DroneMesh->SetStaticMesh(SphereMesh.Object);
    }
}

void ACDRQuestTrainingDrone::Interact_Implementation(AActor* Interactor)
{
    if (!IsValid(Interactor))
    {
        return;
    }

    UCDRQuestComponent* QuestComponent =
        Interactor->FindComponentByClass<UCDRQuestComponent>();

    if (!IsValid(QuestComponent))
    {
        return;
    }

    FCDRQuestRuntime QuestData;
    int32 QuestIndex = INDEX_NONE;

    const bool bMissionIsActive = QuestComponent->FindQuestByID(
        TEXT("Q_CPP_FieldOperations"),
        QuestData,
        QuestIndex);

    if (!bMissionIsActive || !QuestData.bIsActive || QuestData.bIsCompleted)
    {
        return;
    }

    ACDRQuestGameState* QuestGameState =
        GetWorld()->GetGameState<ACDRQuestGameState>();

    if (!IsValid(QuestGameState))
    {
        return;
    }

    QuestGameState->ReportGameplayObjective(
        ECDRQuestObjectiveType::Kill,
        TargetID,
        1);

    Destroy();
}

