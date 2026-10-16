#include "CRCoinLeaderboardItemWidget.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "UI/CoinLeaderboard/CRCoinLeaderboardItem.h"

void UCRCoinLeaderboardItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// UCRCoinLeaderboardItem 캐스팅
	UCRCoinLeaderboardItem* CoinLeaderboardItem = Cast<UCRCoinLeaderboardItem>(ListItemObject);

	if (IsValid(CoinLeaderboardItem) == false)
	{
		return;
	}

	// 코인 리더보드 데이터
	const FCoinLeaderboardEntry CoinLeaderboardData = CoinLeaderboardItem->GetCoinLeaderboardData();

	// TextBlock_Rank 위젯 설정
	TextBlock_Rank->SetText(FText::AsNumber(CoinLeaderboardData.Rank));
	// TextBlock_PlayerName 위젯 설정
	TextBlock_PlayerName->SetText(FText::FromString(CoinLeaderboardData.PlayerName));
	// TextBlock_Coins 위젯 설정
	TextBlock_Coins->SetText(FText::AsNumber(CoinLeaderboardData.Coins));

	// 배경 설정
	if (CoinLeaderboardItem->IsOwner() == true) // 하이라이트 On
	{
		Border_NormalBackground->SetVisibility(ESlateVisibility::Hidden);
		Border_HighlightBackground->SetVisibility(ESlateVisibility::Visible);
	}
	else // 하이라이트 Off
	{
		Border_NormalBackground->SetVisibility(ESlateVisibility::Visible);
		Border_HighlightBackground->SetVisibility(ESlateVisibility::Hidden);
	}
}
