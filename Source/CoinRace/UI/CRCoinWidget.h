#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CRCoinWidget.generated.h"

class UTextBlock;
class ACRPlayerState;

UCLASS()
class COINRACE_API UCRCoinWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Coins;

	FDelegateHandle CoinsUpdatedHandle;

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

  public:
	void SetCoins(int32 Amount);

  protected:
	UFUNCTION()
	void HandleCoinsUpdated(const ACRPlayerState* PlayerState, int32 NewCoins);
};
