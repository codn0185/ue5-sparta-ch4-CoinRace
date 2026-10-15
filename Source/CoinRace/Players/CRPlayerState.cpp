#include "CRPlayerState.h"

#include "Net/UnrealNetwork.h"

void ACRPlayerState::OnRep_Coins()
{
	OnCoinsChanged.ExecuteIfBound(Coins);
}

void ACRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME_CONDITION(ThisClass, Coins, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, BonusMaxSpeed, COND_OwnerOnly);
}

void ACRPlayerState::ServerRPCSetPlayerName_Implementation(const FString& InPlayerName)
{
	SetPlayerName(InPlayerName);
}

void ACRPlayerState::ServerRPCAddCoins_Implementation(int32 Amount)
{
	AddCoins(Amount);
}

void ACRPlayerState::ServerRPCRemoveCoins_Implementation(int32 Amount)
{
	TryRemoveCoins(Amount);
}

void ACRPlayerState::ServerRPCAddBonusMaxSpeed_Implementation(float Amount)
{
	AddBonusMaxSpeed(Amount);
}

FString ACRPlayerState::GetPlayerName() const
{
	return PlayerName;
}

void ACRPlayerState::SetPlayerName(const FString& InPlayerName)
{
	PlayerName = InPlayerName;
}

int32 ACRPlayerState::GetCoins() const
{
	return Coins;
}

void ACRPlayerState::AddCoins(int32 Amount)
{
	// 서버에서만 수정 가능하도록 제한
	if (HasAuthority() == false)
	{
		return;
	}

	if (Amount <= 0)
	{
		return;
	}

	Coins += Amount;
}

bool ACRPlayerState::TryRemoveCoins(int32 Amount)
{
	// 서버에서만 수정 가능하도록 제한
	if (HasAuthority() == false)
	{
		return false;
	}

	if (Amount <= 0 || Coins <= Amount)
	{
		return false;
	}

	Coins -= Amount;

	return true;
}

float ACRPlayerState::GetBonusMaxSpeed() const
{
	return BonusMaxSpeed;
}

void ACRPlayerState::AddBonusMaxSpeed(float Amount)
{
	// 서버에서만 수정 가능하도록 제한
	if (HasAuthority() == false)
	{
		return;
	}

	if (Amount <= 0.f)
	{
		return;
	}

	BonusMaxSpeed += Amount;
}
