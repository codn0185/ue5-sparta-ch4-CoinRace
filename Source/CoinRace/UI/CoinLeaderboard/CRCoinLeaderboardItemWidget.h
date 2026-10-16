#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"

#include "CRCoinLeaderboardItemWidget.generated.h"

class UTextBlock;
class UBorder;

UCLASS()
class COINRACE_API UCRCoinLeaderboardItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

  protected:
	// 순위
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Rank;

	// 플레이어 이름
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_PlayerName;

	// 코인 개수
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Coins;

	// 일반 배경
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_NormalBackground;

	// 하이라이트 배경
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border_HighlightBackground;

  protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
