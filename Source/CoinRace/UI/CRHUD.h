#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CRHUD.generated.h"

class UCRVehicleStatusWidget;
class UCRChatWidget;
class UCRCoinWidget;

struct FChatData;

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
	TObjectPtr<UCRVehicleStatusWidget> VehicleStatusWidget;

	// UCRChatWidget - 채팅 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCRChatWidget> ChatWidget;

	// UCRCoinWidget - 코인 UI
	TObjectPtr<UCRCoinWidget> CRCoinWidget;

  protected:
	virtual void NativeConstruct() override;

  public:
	// UCRVehicleStatusWidget의 속도 UI 변경
	void UpdateSpeed(float NewSpeed);

	// UCRVehicleStatusWidget의 기어 UI 변경
	void UpdateGear(int32 NewGear);

	// UCRChatWidget 채팅 추가
	void AddChatMessage(const FChatData& ChatData);

	// UCRChatWidget 채팅 클리어
	void ClearChatMessages();

	// UCRChatWidget 채팅 입력창 포커스
	void FocusChatInput();

	// UCRCoinWidget 코인 설정
	void SetCoins(int32 Amount);
};
