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

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

  public:
	void SetCoins(int32 Amount);

	/** OnCoinsUpdated 델리게이트 관리 */
  protected:
	FDelegateHandle CoinsUpdatedHandle;

	UFUNCTION()
	void HandleCoinsUpdated(const ACRPlayerState* PlayerState, int32 NewCoins);
};
