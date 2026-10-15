#include "UI/CRCoinWidget.h"

#include "Components/TextBlock.h"

void UCRCoinWidget::SetCoins(int32 Amount)
{
	TextBlock_Coins->SetText(FText::AsNumber(Amount));
}
