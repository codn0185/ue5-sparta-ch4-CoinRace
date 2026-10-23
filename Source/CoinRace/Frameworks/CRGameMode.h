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

  public:
	ACRGameMode();

  protected:
	// 플레이어 로그인
	virtual void OnPostLogin(AController* NewPlayer) override;
	// 플레이어 로그아웃
	virtual void Logout(AController* Exiting) override;

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
