#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Utilities/SpeedUnit.h"

#include "CRHUD.generated.h"

struct FChatEntry;
struct FCoinLeaderboardEntry;

// 채팅 입력 커밋 시 메시지를 송출하는 델리게이트
DECLARE_DELEGATE_OneParam(
	FOnChatMessageSubmitted,
	const FString&);

UCLASS()
class COINRACE_API UCRHUD : public UUserWidget
{
	GENERATED_BODY()

  public:
	// 채팅 입력 커밋 시 실행되는 델리게이트
	FOnChatMessageSubmitted OnChatMessageSubmitted;

  protected:
	// UCRVehicleStatusWidget - 차량 정보 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCRVehicleStatusWidget> VehicleStatusWidget;

	// UCRChatWidget - 채팅 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCRChatWidget> ChatWidget;

	// UCRCoinWidget - 코인 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCRCoinWidget> CoinWidget;

	// UCRBonusMaxSpeedWidget - 보너스 최대 속도 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCRBonusMaxSpeedWidget> BonusMaxSpeedWidget;

	// UCRCoinLeaderboardWidget - 코인 리더보드 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UCRCoinLeaderboardWidget> CoinLeaderboardWidget;

  protected:
	// UI에 표시할 속도 단위
	FSpeedUnit UISpeedUnit = SpeedUnit::KPH;

  protected:
	virtual void NativeConstruct() override;

  public:
	// UI 속도 단위 설정
	void SetSpeedUnit(const FSpeedUnit& NewSpeedUnit);

	// UCRVehicleStatusWidget의 속도 UI 변경
	void UpdateSpeed(float NewSpeed);
	// UCRVehicleStatusWidget의 기어 UI 변경
	void UpdateGear(int32 NewGear);

	// UCRChatWidget 채팅 추가
	void AddChatMessage(const FChatEntry& ChatData);
	// UCRChatWidget 채팅 클리어
	void ClearChatMessages();
	// UCRChatWidget 채팅 입력창 포커스
	void FocusChatInput();

	// UCRCoinWidget 코인 설정
	void SetCoins(int32 Amount);

	// UCRBonusMaxSpeedWidget 보너스 최대 속도 설정
	void SetBonusMaxSpeed(float NewBonusMaxSpeed);

	// UCRCoinLeaderboardWidget 코인 리더보드 업데이트
	void UpdateCoinLeaderboard(const TArray<FCoinLeaderboardEntry>& CoinLeaderboard);
	// UCRCoinLeaderboardWidget 코인 리더보드 초기화
	void ClearCoinLeaderboard();
};
