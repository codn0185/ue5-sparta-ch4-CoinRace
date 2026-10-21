#include "CRGameState.h"

#include "Data/ChatEntry.h"
#include "Data/CoinLeaderboardEntry.h"
#include "Net/UnrealNetwork.h"

void ACRGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, MaxChatLogCount);
}

void ACRGameState::AddChatData(const FChatEntry& NewChatData)
{
	if (ChatLog.Num() >= MaxChatLogCount)
	{
		ChatLog.RemoveAt(0);
	}

	ChatLog.Add(NewChatData);
}

void ACRGameState::SetChatLog(const TArray<FChatEntry>& InChatLog)
{
	ChatLog = InChatLog;
}

void ACRGameState::ClearChatLog()
{
	ChatLog.Empty();
}

const TArray<FChatEntry>& ACRGameState::GetChatLog() const
{
	return ChatLog;
}

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
	NewEntry.PlayerId = PlayerId;
	NewEntry.PlayerName = PlayerName;
	NewEntry.Coins = 0;

	// 새 항목 추가
	const int32 NewIndex = CoinLeaderboard.Add(NewEntry);
	CoinLeaderboardIndexMap.Add(PlayerId, NewIndex);

	// 순위 갱신
	RefreshCoinLeaderboardRanks();
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

	// 뒷 순위 인덱스 갱신
	for (int32 Index = RemoveIndex; Index < CoinLeaderboard.Num(); ++Index)
	{
		CoinLeaderboardIndexMap[CoinLeaderboard[Index].PlayerId] = Index;
	}

	// 순위 갱신
	RefreshCoinLeaderboardRanks();
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
		if (CoinLeaderboard[CurrentIndex].Coins < CoinLeaderboard[PreviousIndex].Coins)
		{
			break;
		}

		// 플레이어 교체
		CoinLeaderboard.Swap(CurrentIndex, PreviousIndex);

		// 인덱스 갱신
		CoinLeaderboardIndexMap[CoinLeaderboard[CurrentIndex].PlayerId] = CurrentIndex;
		CoinLeaderboardIndexMap[CoinLeaderboard[PreviousIndex].PlayerId] = PreviousIndex;

		--CurrentIndex;
	}

	// 코인 감소 -> 하위 순위로 이동
	// 동점이면 해당 공동 순위 그룹의 첫 번째에 위치
	while (CurrentIndex < CoinLeaderboard.Num() - 1)
	{
		const int32 NextIndex = CurrentIndex + 1;

		// 다음 플레이어보다 코인이 적지 않으면 이동 종료
		if (CoinLeaderboard[CurrentIndex].Coins > CoinLeaderboard[NextIndex].Coins)
		{
			break;
		}

		// 플레이어 교체
		CoinLeaderboard.Swap(CurrentIndex, NextIndex);

		// 인덱스 갱신
		CoinLeaderboardIndexMap[CoinLeaderboard[CurrentIndex].PlayerId] = CurrentIndex;
		CoinLeaderboardIndexMap[CoinLeaderboard[NextIndex].PlayerId] = NextIndex;

		++CurrentIndex;
	}

	// 순위 갱신
	RefreshCoinLeaderboardRanks();
}

void ACRGameState::RefreshCoinLeaderboardRanks()
{
	for (int32 Index = 0; Index < CoinLeaderboard.Num(); ++Index)
	{
		// 동점이면 같은 순위
		if (Index > 0 && CoinLeaderboard[Index].Coins == CoinLeaderboard[Index - 1].Coins)
		{
			CoinLeaderboard[Index].Rank = CoinLeaderboard[Index - 1].Rank;
		}
		else
		{
			CoinLeaderboard[Index].Rank = Index + 1;
		}
	}
}
