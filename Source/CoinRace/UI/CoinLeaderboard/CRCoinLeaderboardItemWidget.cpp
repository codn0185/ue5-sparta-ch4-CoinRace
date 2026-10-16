#include "CRCoinLeaderboardItemWidget.h"

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
	TextBlock_Rank->SetText(FText::FromString(CoinLeaderboardData.PlayerName));
	// TextBlock_Coins 위젯 설정
	TextBlock_Rank->SetText(FText::AsNumber(CoinLeaderboardData.Coins));
}
