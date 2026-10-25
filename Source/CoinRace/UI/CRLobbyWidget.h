#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CRLobbyWidget.generated.h"

class UTextBlock;

UCLASS()
class COINRACE_API UCRLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	// 남은 대기 시간 표시
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_WaitingTime;

	// 플레이어 수 표시
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayerCount;

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

  public:
	// 남은 대기 시간 UI 업데이트
	void UpdateWaitingTime(float WaitingTime);

	// 플레이어 수 UI 업데이트
	void UpdatePlayerCount(int32 PlayerCount);
};
