#include "CRPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "Systems/GameEventSubsystem.h"

void ACRPlayerState::OnRep_Coins()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnCoinsUpdated.Broadcast(this, Coins);
		}
	}
}

void ACRPlayerState::OnRep_BonusMaxSpeed()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnBonusMaxSpeedUpdated.Broadcast(this, BonusMaxSpeed);
		}
	}
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
