#include "QuestCPP/CDRQuestGiver.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "QuestCPP/CDRQuestComponent.h"
#include "QuestCPP/CDRQuestDefinition.h"
#include "UObject/ConstructorHelpers.h"

ACDRQuestGiver::ACDRQuestGiver()
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

	GiverMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("GiverMesh"));

	GiverMesh->SetupAttachment(RootComponent);
	GiverMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GiverMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	GiverMesh->SetRelativeScale3D(FVector(1.2f, 1.2f, 2.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube"));

	if (CubeMesh.Succeeded())
	{
		GiverMesh->SetStaticMesh(CubeMesh.Object);
	}
}

void ACDRQuestGiver::Interact_Implementation(AActor* Interactor)
{
	if (!IsValid(Interactor) || !IsValid(QuestDefinition))
	{
		return;
	}

	UCDRQuestComponent* QuestComponent =
		Interactor->FindComponentByClass<UCDRQuestComponent>();

	if (IsValid(QuestComponent))
	{
		QuestComponent->AddQuest(QuestDefinition);
	}
}
