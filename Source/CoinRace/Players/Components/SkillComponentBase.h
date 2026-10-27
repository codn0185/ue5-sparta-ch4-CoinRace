#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "SkillComponentBase.generated.h"

UENUM()
enum class ESkillType : uint8
{
	None,
	StealCoin,
	DeployMine,
};

UCLASS(Abstract, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COINRACE_API USkillComponentBase : public UActorComponent
{
	GENERATED_BODY()

  protected:
	// 스킬 타입
	ESkillType SkillType = ESkillType::None;

	// 사용 간격
	float CooldownDuration = 1.f;

	// 사용까지 남은 시간
	UPROPERTY(ReplicatedUsing = OnRep_CooldownRemainingTime)
	float CooldownRemainingTime = 0.f;

	// 남은 시간 갱신용 타이머 핸들
	FTimerHandle CooldownTimerHandle;

	// 남은 시간 갱신 간격
	float CooldownUpdateInterval = 0.1f;

  public:
	USkillComponentBase();

  protected:
	// 레플리케이션 대상 등록
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

  protected:
	UFUNCTION()
	void OnRep_CooldownRemainingTime();

  public:
	// 스킬 사용함을 서버에 알림
	UFUNCTION(Server, Reliable)
	void ServerRPCUse();

  protected:
	// 스킬 활성화
	virtual void ActivateSkill()
		PURE_VIRTUAL(ThisClass::ActivateSkill); // 순수가상함수

	// 스킬 사용 가능 여부 반환
	virtual bool CanActivateSkill() const;

	// 스킬 쿨다운 시작
	virtual void StartCooldown();

	// 스킬 쿨다운 갱신
	virtual void UpdateCooldown();
};
