#include "CRPlayerState.h"

#include "Net/UnrealNetwork.h"

void ACRPlayerState::OnRep_Coins()
{
	OnCoinsChanged.Broadcast(Coins);
}

void ACRPlayerState::OnRep_BonusMaxSpeed()
{
	OnBonusMaxSpeedChanged.Broadcast(BonusMaxSpeed);
}

void ACRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, Coins, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, BonusMaxSpeed, COND_OwnerOnly);
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

	if (Amount <= 0 || Coins < Amount)
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
