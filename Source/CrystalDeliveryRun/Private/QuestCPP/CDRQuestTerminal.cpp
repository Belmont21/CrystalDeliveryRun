#include "QuestCPP/CDRQuestTerminal.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "QuestCPP/CDRQuestGameState.h"
#include "UObject/ConstructorHelpers.h"

ACDRQuestTerminal::ACDRQuestTerminal()
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

	TerminalMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("TerminalMesh"));

	TerminalMesh->SetupAttachment(RootComponent);
	TerminalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TerminalMesh->SetRelativeScale3D(FVector(1.4f, 0.8f, 1.8f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube"));

	if (CubeMesh.Succeeded())
	{
		TerminalMesh->SetStaticMesh(CubeMesh.Object);
	}
}

void ACDRQuestTerminal::Interact_Implementation(AActor* Interactor)
{
	ACDRQuestGameState* QuestGameState =
		GetWorld()->GetGameState<ACDRQuestGameState>();

	if (IsValid(QuestGameState))
	{
		QuestGameState->ReportGameplayObjective(
			ECDRQuestObjectiveType::Interact,
			TargetID,
			1);
	}
}
