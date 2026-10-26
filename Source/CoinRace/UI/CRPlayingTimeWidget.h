#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CRPlayingTimeWidget.generated.h"

class UTextBlock;
class ACRPlayerState;

UCLASS()
class COINRACE_API UCRPlayingTimeWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	// 남은 게임 시간 표시 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayingTime;

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

  public:
	void UpdatePlayingTime(float PlayingTime);

  protected:
	UFUNCTION()
	void HandlePlayingTimeRemainingUpdated(float NewRemainingTime);
};
