#include "CRPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "Systems/GameEventSubsystem.h"

void ACRPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// GameEventSubsystem 설정
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
	}
}

void ACRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, GameResult, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, Coins, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, BonusMaxSpeed, COND_OwnerOnly);
}

void ACRPlayerState::OnRep_GameResult()
{
	// 클라이언트에서 OnGameResultUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnGameResultUpdated.Broadcast(this, GameResult);
	}
}

EGameResult ACRPlayerState::GetGameResult() const
{
	return GameResult;
}

void ACRPlayerState::SetGameResult(EGameResult NewGameResult)
{
	// 서버에서만 수정 가능하도록 제한
	if (HasAuthority() == false)
	{
		return;
	}

	GameResult = NewGameResult;

	// 서버에서 OnGameResultUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnGameResultUpdated.Broadcast(this, GameResult);
	}
}

void ACRPlayerState::OnRep_Coins()
{
	// 클라이언트에서 OnCoinsUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnCoinsUpdated.Broadcast(this, Coins);
	}
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

	// 서버에서 OnCoinsUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnCoinsUpdated.Broadcast(this, Coins);
	}
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

	// 서버에서 OnCoinsUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnCoinsUpdated.Broadcast(this, Coins);
	}

	return true;
}

void ACRPlayerState::OnRep_BonusMaxSpeed()
{
	// 클라이언트에서 OnBonusMaxSpeedUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnBonusMaxSpeedUpdated.Broadcast(this, BonusMaxSpeed);
	}
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

	// 서버에서 OnBonusMaxSpeedUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnBonusMaxSpeedUpdated.Broadcast(this, BonusMaxSpeed);
	}
}

void ACRPlayerState::ReduceBonusMaxSpeed(float Amount)
{ // 서버에서만 수정 가능하도록 제한
	if (HasAuthority() == false)
	{
		return;
	}

	if (Amount <= 0.f)
	{
		return;
	}

	BonusMaxSpeed -= Amount;

	// 서버에서 OnBonusMaxSpeedUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnBonusMaxSpeedUpdated.Broadcast(this, BonusMaxSpeed);
	}

	// 타이머 활성화 - 초당 1 회복 반복
	GetWorldTimerManager().SetTimer(
		RecoveryBonusMaxSpeedTimerHandle,
		FTimerDelegate::CreateLambda(
			[this]()
			{
				// 회복 중단
				if (BonusMaxSpeed >= 0.f)
				{
					GetWorld()->GetTimerManager().ClearTimer(RecoveryBonusMaxSpeedTimerHandle);
					return;
				}

				// 0 초과 방지
				BonusMaxSpeed = FMath::Min(BonusMaxSpeed + 1.f, 0.f);
			}),
		1.0f,
		true);
}
