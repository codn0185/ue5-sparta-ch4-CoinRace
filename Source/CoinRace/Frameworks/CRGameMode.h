#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "CRGameMode.generated.h"

struct FChatEntry;

class ACRPlayerController;

UCLASS(MinimalAPI)
class ACRGameMode : public AGameModeBase
{
	GENERATED_BODY()

  protected:
	TArray<TObjectPtr<ACRPlayerController>> AllPlayerControllers;

	// 대기 시간
	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ClampMin = 1.0, UIMin = 1.0))
	float WaitingTime = 5.f;

	// 게임 시간
	UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ClampMin = 1.0, UIMin = 1.0))
	float PlayinigTime = 30.f;

	// 대기 타이머 핸들
	FTimerHandle WaitingTimerHandle;

	// 게임 타이머 핸들
	FTimerHandle PlayingTimerHandle;

  public:
	ACRGameMode();

  protected:
	// 플레이어 로그인
	virtual void OnPostLogin(AController* NewPlayer) override;
	// 플레이어 로그아웃
	virtual void Logout(AController* Exiting) override;

  protected:
	// 시작
	virtual void BeginPlay() override;

	// 대기 카운드다운 시작
	void StartWaitingCountdown();

	// 대기 시간 업데이트
	void UpdateWaitingCountdown();

	// 게임 시작
	void StartGame();

	// 게임 시간 업데이트
	void UpdatePlayingCountdown();

	// 게임 종료
	void FinishGame();

  public:
	// 플레이어 초기화 완료 후 호출
	void OnPlayerInitialized(ACRPlayerController* Player);

  protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

  private:
	bool IsPlayerStartAvailable(const APlayerStart* PlayerStart) const;

  private:
	UPROPERTY(EditDefaultsOnly)
	FVector VehicleSpawnCheckHalfExtent = FVector(250.f, 120.f, 80.f);

	UPROPERTY(EditDefaultsOnly)
	FVector VehicleSpawnCheckOffset = FVector(00.f, 00.f, 80.f);

  public:
	// 채팅 입력 시 호출되어 모든 플레이어에게 전달
	void BroadcastChatMessage(ACRPlayerController* SenderController, const FString& ChatMessage) const;
};
