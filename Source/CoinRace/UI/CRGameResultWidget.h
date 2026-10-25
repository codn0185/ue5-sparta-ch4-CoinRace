#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CRGameResultWidget.generated.h"

class UPanelWidget;
class ACRPlayerState;
enum class EGameResult : uint8;

UCLASS()
class COINRACE_API UCRGameResultWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	// 승리 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> PanelWidget_VictoryUI;

	// 패배 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> PanelWidget_DefeatUI;

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

  protected:
	UFUNCTION()
	void HandleGameResultUpdated(const ACRPlayerState* PlayerState, EGameResult NewGameResult);
};
