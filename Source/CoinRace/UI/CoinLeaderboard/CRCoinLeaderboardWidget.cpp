#include "CRCoinLeaderboardWidget.h"

#include "Components/ListView.h"
#include "GameFramework/PlayerState.h"
#include "UI/CoinLeaderboard/CRCoinLeaderboardItem.h"

void UCRCoinLeaderboardWidget::UpdateCoinLeaderboard(const TArray<FCoinLeaderboardEntry>& CoinLeaderboard)
{
	// 플레이어 ID 확인
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	if (IsValid(PlayerState) == false)
	{
		return;
	}

	const int32 PlayerId = PlayerState->GetPlayerId();

	// 코인 리더보드 초기화
	ClearCoinLeaderboard();

	// 상위 순위 순서대로 추가
	for (const FCoinLeaderboardEntry& CoinLeaderboardData : CoinLeaderboard)
	{
		UCRCoinLeaderboardItem* CoinLeaderboardItem = NewObject<UCRCoinLeaderboardItem>(this);
		if (IsValid(CoinLeaderboardItem) == true)
		{
			CoinLeaderboardItem->SetCoinLeaderboardData(CoinLeaderboardData);
			CoinLeaderboardItem->SetIsOwner(PlayerId == CoinLeaderboardData.PlayerId);

			ListView_CoinLeaderboard->AddItem(CoinLeaderboardItem);
		}
	}
}

void UCRCoinLeaderboardWidget::ClearCoinLeaderboard()
{
	ListView_CoinLeaderboard->ClearListItems();
}
