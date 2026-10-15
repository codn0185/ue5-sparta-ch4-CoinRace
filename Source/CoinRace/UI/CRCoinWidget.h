#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CRCoinWidget.generated.h"

class UTextBlock;

UCLASS()
class COINRACE_API UCRCoinWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Coins;

  protected:
	virtual void NativeConstruct() override;

  public:
	void SetCoins(int32 Amount);
};
