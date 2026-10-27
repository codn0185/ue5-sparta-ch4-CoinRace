#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"

#include "Interfaces/AttackableInterface.h"
#include "Interfaces/ItemEffectReceiverInterface.h"

#include "CRWheeledVehiclePawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UChaosWheeledVehicleMovementComponent;
class UStealCoinSkillComponent;
class UInputAction;
class ACRPlayerState;
class AMineItem;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateVehicle, Log, All);

UCLASS()
class COINRACE_API ACRWheeledVehiclePawn : public AWheeledVehiclePawn,
										   public IAttackableInterface,
										   public IItemEffectReceiverInterface
{
	GENERATED_BODY()

  protected:
	/** Spring Arm for the front camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* FrontSpringArm;

	/** Front Camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FrontCamera;

	/** Spring Arm for the back camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* BackSpringArm;

	/** Back Camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* BackCamera;

	/** Cast pointer to the Chaos Vehicle movement component */
	TObjectPtr<UChaosWheeledVehicleMovementComponent> ChaosVehicleMovement;

	/** 스킬 컴포넌트 */
  protected:
	// 공격하여 코인을 빼앗는 스킬 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStealCoinSkillComponent> StealCoinSkillComponent;

  protected:
	/** Steering Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SteeringAction;

	/** Throttle Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ThrottleAction;

	/** Brake Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* BrakeAction;

	/** Handbrake Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HandbrakeAction;

	/** Look Around Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAroundAction;

	/** Toggle Camera Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ToggleCameraAction;

	/** Reset Vehicle Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ResetVehicleAction;

	/** Focus Chat Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FocusChatAction;

	/** Attack Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;

	/** Deploy Mine Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DeployMineAction;

	/** Keeps track of which camera is active */
	bool bFrontCameraActive = false;

  protected:
	// ACRPlayerState 포인터 캐시
	TObjectPtr<ACRPlayerState> CRPlayerState;

  protected:
	// 지뢰 아이템 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<AMineItem> MineItemClass;

	// 지뢰 설치 딜레이
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DeployMineCooldown = 2.f;

	// 지뢰 설치 쿨다운 타이머
	FTimerHandle DeployMineCooldownTimerHandle;

  public:
	ACRWheeledVehiclePawn();

	// Begin Pawn interface

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	// End Pawn interface

	// Begin Actor interface

	virtual void Tick(float Delta) override;

	// End Actor interface

  protected:
	/** Handles steering input */
	void Steering(const FInputActionValue& Value);

	/** Handles throttle input */
	void Throttle(const FInputActionValue& Value);

	/** Handles brake input */
	void Brake(const FInputActionValue& Value);

	/** Handles brake start/stop inputs */
	void StartBrake(const FInputActionValue& Value);
	void StopBrake(const FInputActionValue& Value);

	/** Handles handbrake start/stop inputs */
	void StartHandbrake(const FInputActionValue& Value);
	void StopHandbrake(const FInputActionValue& Value);

	/** Handles look around input */
	void LookAround(const FInputActionValue& Value);

	/** Handles toggle camera input */
	void ToggleCamera(const FInputActionValue& Value);

	/** Handles reset vehicle input */
	void ResetVehicle(const FInputActionValue& Value);

	/** Handles focus chat input */
	void FocusChatInput(const FInputActionValue& Value);

	/** Handles attack input */
	void AttackInput(const FInputActionValue& Value);

	/** Handles deploy mine input */
	void DeployMine(const FInputActionValue& Value);

	// 지뢰 배치 입력함을 서버에 알림
	UFUNCTION(Server, Reliable)
	void ServerRPCDeployMine();

	/** Called when the brake lights are turned on or off */
	UFUNCTION(BlueprintImplementableEvent, Category = "Vehicle")
	void BrakeLights(bool bBraking);

  public:
	/** Returns the front spring arm subobject */
	FORCEINLINE USpringArmComponent* GetFrontSpringArm() const
	{
		return FrontSpringArm;
	}
	/** Returns the front camera subobject */
	FORCEINLINE UCameraComponent* GetFollowCamera() const
	{
		return FrontCamera;
	}
	/** Returns the back spring arm subobject */
	FORCEINLINE USpringArmComponent* GetBackSpringArm() const
	{
		return BackSpringArm;
	}
	/** Returns the back camera subobject */
	FORCEINLINE UCameraComponent* GetBackCamera() const
	{
		return BackCamera;
	}
	/** Returns the cast Chaos Vehicle Movement subobject */
	FORCEINLINE const TObjectPtr<UChaosWheeledVehicleMovementComponent>& GetChaosVehicleMovement() const
	{
		return ChaosVehicleMovement;
	}

  protected:
	// 전진 가속 비활성화 타이머 핸들
	FTimerHandle DisableThrottleTimerHandle;

	// 전진 가속 가능 여부
	bool CanThrottle() const;

	/** IAttackableInterface 인터페이스 구현 */
  public:
	// 코인을 빼앗기고, 실제로 빼앗긴 수량을 반환
	virtual int32 StealCoins(int32 Amount) override;

	// 추가 최대 속도 감소 패널티 적용
	virtual void ApplyBonusMaxSpeedPenalty(float Amount) override;

	// 피격 부위 확인 (유효한 피격 부위 여부를 반환)
	virtual bool GetHitBody(const FHitResult& HitResult, EHitPart& OutHitPart) const override;

	/** IItemEffectReceiverInterface 인터페이스 구현 */
  public:
	// 코인 추가
	virtual void AddCoins(int32 Amount) override;

	// 추가 최대 속도 증가
	virtual void AddBonusMaxSpeed(float Amount) override;

	// 전진 가속 비활성화 (가속 페달 고장)
	virtual void DisableThrottle(float Duration) override;
};
