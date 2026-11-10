#include "Multiplayer/CDRGameInstance.h"

#include "Engine/World.h"

void UCDRGameInstance::Init()
{
	Super::Init();

	if (IsValid(GEngine))
	{
		NetworkFailureHandle = GEngine->OnNetworkFailure().AddUObject(
		   this,
		   &UCDRGameInstance::HandleNetworkFailure);
	}
}

void UCDRGameInstance::Shutdown()
{
	if (IsValid(GEngine))
	{
		GEngine->OnNetworkFailure().Remove(NetworkFailureHandle);
	}

	Super::Shutdown();
}

void UCDRGameInstance::HandleNetworkFailure(
   UWorld* World,
   UNetDriver* NetDriver,
   ENetworkFailure::Type FailureType,
   const FString& ErrorString)
{
	
	if (!IsValid(World) || World->GetGameInstance() != this)
	{
		return;
	}

	
	if (World->GetNetMode() == NM_ListenServer ||
		World->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	if (FailureType == ENetworkFailure::ConnectionTimeout)
	{
		PendingNetworkError = FText::FromString(
		   TEXT("No se pudo conectar: tiempo de espera agotado."));
	}
	else if (FailureType == ENetworkFailure::ConnectionLost)
	{
		PendingNetworkError = FText::FromString(
		   TEXT("Se perdió la conexión con el host."));
	}
	else
	{
		PendingNetworkError = FText::FromString(
		   TEXT("No se pudo establecer o mantener la conexión."));
	}
}

FText UCDRGameInstance::ConsumeNetworkError()
{
	const FText ErrorToShow = PendingNetworkError;
	PendingNetworkError = FText::GetEmpty();

	return ErrorToShow;
}
