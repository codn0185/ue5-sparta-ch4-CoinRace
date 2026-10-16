#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"

#include "CRCoinLeaderboardItemWidget.generated.h"

class UTextBlock;

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

  protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
