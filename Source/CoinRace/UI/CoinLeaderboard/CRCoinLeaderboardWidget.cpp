#include "CRCoinLeaderboardWidget.h"

#include "Components/ListView.h"
#include "UI/CoinLeaderboard/CRCoinLeaderboardItem.h"

void UCRCoinLeaderboardWidget::UpdateCoinLeaderboard(const TArray<FCoinLeaderboardEntry>& CoinLeaderboard)
{

	ClearCoinLeaderboard();

	for (const FCoinLeaderboardEntry& CoinLeaderboardData : CoinLeaderboard)
	{
		UCRCoinLeaderboardItem* CoinLeaderboardItem = NewObject<UCRCoinLeaderboardItem>(this);
		if (IsValid(CoinLeaderboardItem) == true)
		{
			CoinLeaderboardItem->SetCoinLeaderboardData(CoinLeaderboardData);
			ListView_CoinLeaderboard->AddItem(CoinLeaderboardItem);
		}
	}
}

void UCRCoinLeaderboardWidget::ClearCoinLeaderboard()
{
	ListView_CoinLeaderboard->ClearListItems();
}
