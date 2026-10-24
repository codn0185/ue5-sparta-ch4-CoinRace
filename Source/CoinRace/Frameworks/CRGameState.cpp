#include "CRGameState.h"

#include "Data/ChatEntry.h"
#include "Data/CoinLeaderboardEntry.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Net/UnrealNetwork.h"
#include "Players/CRPlayerState.h"
#include "Systems/GameEventSubsystem.h"

void ACRGameState::BeginPlay()
{
	Super::BeginPlay();

	// GameEventSubsystem 설정
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
	}

	// GameState에 영향을 주는 게임 이벤트 바인딩
	if (IsValid(GameEventSubsystem) == true)
	{
		// 약한 참조
		TWeakObjectPtr<ACRGameState> WeakThis(this);

		// 코인 변경 이벤트를 수신하여 GameState의 코인 리더보드 갱신
		GameEventSubsystem->OnCoinsUpdated.AddLambda(
			[WeakThis](const ACRPlayerState* CRPS, int32 NewCoins)
			{
				if (WeakThis.IsValid() == true && IsValid(CRPS) == true)
				{
					WeakThis->UpdateCoinLeaderboard(CRPS->GetPlayerId(), NewCoins);
				}
			});
	}
}

void ACRGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentGamePhase);
	DOREPLIFETIME(ThisClass, WaitingTimeRemaining);
	DOREPLIFETIME(ThisClass, MaxChatLogCount);
	DOREPLIFETIME(ThisClass, CoinLeaderboard);
}

void ACRGameState::OnRep_CurrentGamePhase()
{
	// 클라이언트에서 OnGamePhaseUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnGamePhaseUpdated.Broadcast(CurrentGamePhase);
	}
}

EGamePhase ACRGameState::GetCurrentGamePhase() const
{
	return CurrentGamePhase;
}

void ACRGameState::ChangeGamePhase(EGamePhase NewGamePhase)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// GamePhase 변화 없음
	if (CurrentGamePhase == NewGamePhase)
	{
		return;
	}

	// GamePhase 업데이트
	CurrentGamePhase = NewGamePhase;

	// 서버에서 OnGamePhaseUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnGamePhaseUpdated.Broadcast(CurrentGamePhase);
	}
}

void ACRGameState::OnRep_WaitingTimeRemaining()
{
	// 클라이언트에서 OnRemainingTimeUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnWaitingTimeRemainingUpdated.Broadcast(WaitingTimeRemaining);
	}
}

float ACRGameState::GetWaitingTimeRemaining() const
{
	return WaitingTimeRemaining;
}

void ACRGameState::SetWaitingTimeRemaining(float NewRemainingTime)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// WaitingTimeRemaining 설정 (음수 방지)
	WaitingTimeRemaining = FMath::Max(0.f, NewRemainingTime);

	// 서버에서 OnWaitingTimeRemainingUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnWaitingTimeRemainingUpdated.Broadcast(WaitingTimeRemaining);
	}
}

void ACRGameState::OnRep_PlayingTimeRemaining()
{
	// 클라이언트에서 OnPlayingTimeRemainingUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnPlayingTimeRemainingUpdated.Broadcast(PlayingTimeRemaining);
	}
}

float ACRGameState::GetPlayingTimeRemaining() const
{
	return PlayingTimeRemaining;
}

void ACRGameState::SetPlayingTimeRemaining(float NewRemainingTime)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	PlayingTimeRemaining = NewRemainingTime;

	// 서버에서 OnPlayingTimeRemainingUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnPlayingTimeRemainingUpdated.Broadcast(PlayingTimeRemaining);
	}
}

void ACRGameState::OnRep_WinnerPlayerIds()
{
	// 클라이언트에서 OnWinnerPlayerIdsUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnWinnerPlayerIdsUpdated.Broadcast(WinnerPlayerIds);
	}
}

const TArray<int32>& ACRGameState::GetWinnerPlayerIds() const
{
	return WinnerPlayerIds;
}

void ACRGameState::SetWinnerPlayerIds(const TArray<int32>& NewWinnerPlayerIds)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	WinnerPlayerIds = NewWinnerPlayerIds;

	// 서버에서 OnWinnerPlayerIdsUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnWinnerPlayerIdsUpdated.Broadcast(WinnerPlayerIds);
	}
}

void ACRGameState::ClearWinnerPlayerIds()
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	WinnerPlayerIds.Empty();

	// 서버에서 OnWinnerPlayerIdsUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnWinnerPlayerIdsUpdated.Broadcast(WinnerPlayerIds);
	}
}

void ACRGameState::AddChatData(const FChatEntry& NewChatData)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// 최대 보관 개수 초과 시 오래된 로그 제거
	if (ChatLog.Num() >= MaxChatLogCount)
	{
		ChatLog.RemoveAt(0);
	}

	// 채팅 로그 추가
	ChatLog.Add(NewChatData);
}

void ACRGameState::SetChatLog(const TArray<FChatEntry>& InChatLog)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// 새 채팅 로그로 덮어씌우기
	ChatLog = InChatLog;
}

void ACRGameState::ClearChatLog()
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// 채팅 로그 클리어
	ChatLog.Empty();
}

const TArray<FChatEntry>& ACRGameState::GetChatLog() const
{
	return ChatLog;
}

void ACRGameState::OnRep_CoinLeaderboard()
{
	// 클라이언트에서 OnCoinLeaderboardUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnCoinLeaderboardUpdated.Broadcast(CoinLeaderboard);
	}
}

const TArray<FCoinLeaderboardEntry>& ACRGameState::GetCoinLeaderboard() const
{
	return CoinLeaderboard;
}

void ACRGameState::ClearCoinLeaderboard()
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// CoinLeaderboard & CoinLeaderboardIndexMap 클리어
	CoinLeaderboard.Empty();
	CoinLeaderboardIndexMap.Empty();

	// 서버에서 OnCoinLeaderboardUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnCoinLeaderboardUpdated.Broadcast(CoinLeaderboard);
	}
}

void ACRGameState::AddPlayerToCoinLeaderboard(const int32 PlayerId, const FString& PlayerName)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

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

	// 서버에서 OnCoinLeaderboardUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnCoinLeaderboardUpdated.Broadcast(CoinLeaderboard);
	}
}

void ACRGameState::RemovePlayerFromCoinLeaderboard(const int32 PlayerId)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

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

	// 서버에서 OnCoinLeaderboardUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnCoinLeaderboardUpdated.Broadcast(CoinLeaderboard);
	}
}

void ACRGameState::UpdateCoinLeaderboard(const int32 PlayerId, const int32 NewCoins)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

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

	// 서버에서 OnCoinLeaderboardUpdated 델리게이트 송출
	if (IsValid(GameEventSubsystem) == true)
	{
		GameEventSubsystem->OnCoinLeaderboardUpdated.Broadcast(CoinLeaderboard);
	}

	// CoinLeaderboard의 변경 사항이 레플리케이션되도록 설정
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, CoinLeaderboard, this);
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
