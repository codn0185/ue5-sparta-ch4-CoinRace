#include "CRGameState.h"

const TArray<FCoinLeaderboardEntry>& ACRGameState::GetCoinLeaderboard() const
{
	return CoinLeaderboard;
}

void ACRGameState::ClearCoinLeaderboard()
{
	CoinLeaderboard.Empty();
	CoinLeaderboardIndexMap.Empty();
}

void ACRGameState::AddPlayerToCoinLeaderboard(const int32 PlayerId, const FString& PlayerName)
{
	// 플레이어 중복
	if (CoinLeaderboardIndexMap.Contains(PlayerId) == true)
	{
		return;
	}

	// 코인 리더보드 항목 생성
	FCoinLeaderboardEntry NewEntry;
	NewEntry.Rank = CoinLeaderboard.Num() + 1;
	NewEntry.PlayerId = PlayerId;
	NewEntry.PlayerName = PlayerName;
	NewEntry.Coins = 0;

	// 동점 시 공동 순위 설정
	if (CoinLeaderboard.IsEmpty() == false && CoinLeaderboard.Last().Coins == NewEntry.Coins)
	{
		NewEntry.Rank = CoinLeaderboard.Last().Rank;
	}

	// 새 항목 추가
	CoinLeaderboard.Add(NewEntry);
	CoinLeaderboardIndexMap.Add(PlayerId, CoinLeaderboard.Num() - 1);
}

void ACRGameState::RemovePlayerFromCoinLeaderboard(const int32 PlayerId)
{
	// 플레이어 없음
	if (CoinLeaderboardIndexMap.Contains(PlayerId) == false)
	{
		return;
	}

	// 제거할 위치
	const int32 RemoveIndex = CoinLeaderboardIndexMap[PlayerId];

	// 플레이어 제거
	CoinLeaderboard.RemoveAt(RemoveIndex);
	CoinLeaderboardIndexMap.Remove(PlayerId);

	// 순위 및 인덱스 갱신
	for (int32 Index = RemoveIndex; Index < CoinLeaderboard.Num(); ++Index)
	{
		// 동점자는 같은 순위
		if (Index > 0 && CoinLeaderboard[Index].Coins == CoinLeaderboard[Index - 1].Coins)
		{
			CoinLeaderboard[Index].Rank = CoinLeaderboard[Index - 1].Rank;
		}
		else
		{
			CoinLeaderboard[Index].Rank = Index + 1;
		}

		// 인덱스 갱신
		CoinLeaderboardIndexMap[CoinLeaderboard[Index].PlayerId] = Index;
	}
}

void ACRGameState::UpdateCoinLeaderboard(const int32 PlayerId, const int32 NewCoins)
{
	// 플레이어 없음
	if (CoinLeaderboardIndexMap.Contains(PlayerId) == false)
	{
		return;
	}

	// 업데이트할 플레이어 인덱스
	int32 CurrentIndex = CoinLeaderboardIndexMap[PlayerId];

	// 코인 수 변경
	CoinLeaderboard[CurrentIndex].Coins = NewCoins;

	// 코인 증가 -> 상위 순위로 이동
	// 동점이면 해당 공동 순위 그룹의 마지막에 위치
	while (CurrentIndex > 0)
	{
		const int32 PreviousIndex = CurrentIndex - 1;

		// 이전 플레이어보다 코인이 많지 않으면 이동 종료
		if (CoinLeaderboard[CurrentIndex].Coins <= CoinLeaderboard[PreviousIndex].Coins)
		{
			break;
		}

		// 플레이어 교체
		CoinLeaderboard.Swap(CurrentIndex, PreviousIndex);

		// 교체된 플레이어 참조
		FCoinLeaderboardEntry& CurrentEntry = CoinLeaderboard[CurrentIndex];
		FCoinLeaderboardEntry& PreviousEntry = CoinLeaderboard[PreviousIndex];

		// 이동한 플레이어의 공동 순위 갱신
		if (PreviousIndex > 0 && PreviousEntry.Coins == CoinLeaderboard[PreviousIndex - 1].Coins)
		{
			PreviousEntry.Rank = CoinLeaderboard[PreviousIndex - 1].Rank;
		}
		else
		{
			PreviousEntry.Rank = PreviousIndex + 1;
		}

		// 밀려난 플레이어의 순위 갱신
		CurrentEntry.Rank = CurrentIndex + 1;

		// 인덱스 갱신
		CoinLeaderboardIndexMap[CurrentEntry.PlayerId] = CurrentIndex;
		CoinLeaderboardIndexMap[PreviousEntry.PlayerId] = PreviousIndex;

		--CurrentIndex;
	}

	// 코인 감소 -> 하위 순위로 이동
	// 동점이면 해당 공동 순위 그룹의 첫 번째에 위치
	while (CurrentIndex < CoinLeaderboard.Num() - 1)
	{
		const int32 NextIndex = CurrentIndex + 1;

		// 다음 플레이어보다 코인이 적지 않으면 이동 종료
		if (CoinLeaderboard[CurrentIndex].Coins >= CoinLeaderboard[NextIndex].Coins)
		{
			break;
		}

		// 플레이어 교체
		CoinLeaderboard.Swap(CurrentIndex, NextIndex);

		// 교체된 플레이어 참조
		FCoinLeaderboardEntry& CurrentEntry = CoinLeaderboard[CurrentIndex];
		FCoinLeaderboardEntry& NextEntry = CoinLeaderboard[NextIndex];

		// 앞으로 이동한 플레이어의 공동 순위 갱신
		if (CurrentIndex > 0 && CurrentEntry.Coins == CoinLeaderboard[CurrentIndex - 1].Coins)
		{
			CurrentEntry.Rank = CoinLeaderboard[CurrentIndex - 1].Rank;
		}
		else
		{
			CurrentEntry.Rank = CurrentIndex + 1;
		}

		// 뒤로 이동한 플레이어의 순위 갱신
		NextEntry.Rank = NextIndex + 1;

		// 인덱스 갱신
		CoinLeaderboardIndexMap[CurrentEntry.PlayerId] = CurrentIndex;
		CoinLeaderboardIndexMap[NextEntry.PlayerId] = NextIndex;

		++CurrentIndex;
	}
}
