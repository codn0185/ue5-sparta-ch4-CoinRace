#include "CRCoinLeaderboardWidget.h"

#include "Components/ListView.h"
#include "UI/CoinLeaderboard/CRCoinLeaderboardItem.h"

void UCRCoinLeaderboardWidget::UpdateCoinLeaderboard(const TArray<FCoinLeaderboardEntry>& CoinLeaderboard)
{
	UCRCoinLeaderboardItem* CoinLeaderboardItem = NewObject<UCRCoinLeaderboardItem>(this);
	if (IsValid(CoinLeaderboardItem) == false)
	{
		return;
	}

	ClearCoinLeaderboard();

	for (const FCoinLeaderboardEntry& CoinLeaderboardData : CoinLeaderboard)
	{
		CoinLeaderboardItem->SetCoinLeaderboardData(CoinLeaderboardData);
		ListView_CoinLeaderboard->AddItem(CoinLeaderboardItem);
	}
}

void UCRCoinLeaderboardWidget::ClearCoinLeaderboard()
{
	ListView_CoinLeaderboard->ClearListItems();
}
