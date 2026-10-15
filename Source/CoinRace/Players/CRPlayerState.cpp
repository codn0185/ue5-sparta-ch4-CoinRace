#include "CRPlayerState.h"

#include "Net/UnrealNetwork.h"

void ACRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerName);
}

FString ACRPlayerState::GetPlayerName() const
{
	return PlayerName;
}

void ACRPlayerState::SetPlayerName(const FString& InPlayerName)
{
	PlayerName = InPlayerName;
}
