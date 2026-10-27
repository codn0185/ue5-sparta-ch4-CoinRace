#include "SkillComponentBase.h"

#include "Net/UnrealNetwork.h"
#include "Systems/GameEventSubsystem.h"

USkillComponentBase::USkillComponentBase()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void USkillComponentBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, CooldownRemainingTime, COND_OwnerOnly);
}

void USkillComponentBase::OnRep_CooldownRemainingTime()
{
	// 클라이언트에서 OnSkillStateUpdated 델리게이트 송출
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnSkillStateUpdated.Broadcast(
				FSkillState(
					SkillType,
					CooldownRemainingTime,
					CanActivateSkill()));
		}
	}
}

void USkillComponentBase::ServerRPCUse_Implementation()
{
	if (CanActivateSkill() == false)
	{
		return;
	}

	ActivateSkill();

	StartCooldown();
}

bool USkillComponentBase::CanActivateSkill() const
{
	// 남은 시간 확인
	if (CooldownRemainingTime <= 0.f ||
		FMath ::IsNearlyZero(CooldownRemainingTime) == true)
	{
		return true;
	}

	return false;
}

void USkillComponentBase::StartCooldown()
{
	// 쿨다운 설정
	CooldownRemainingTime = CooldownDuration;

	// 남은 시간 갱신 타이머 활성화
	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&ThisClass::UpdateCooldown,
		CooldownUpdateInterval,
		true);
}

void USkillComponentBase::UpdateCooldown()
{
	// 남은 시간 갱신
	CooldownRemainingTime = FMath::Max(0.f, CooldownRemainingTime - CooldownUpdateInterval);

	// 사용 가능한 상태가 되면, 타이머 제거
	if (CanActivateSkill() == true)
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	}
}
