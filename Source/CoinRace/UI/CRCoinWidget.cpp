#include "UI/CRCoinWidget.h"

#include "Components/TextBlock.h"

void UCRCoinWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_Coins->SetText(FText::AsNumber(0));
}

void UCRCoinWidget::SetCoins(int32 Amount)
{
	TextBlock_Coins->SetText(FText::AsNumber(Amount));
}
