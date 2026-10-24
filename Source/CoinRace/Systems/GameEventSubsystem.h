#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "GameEventSubsystem.generated.h"

class ACRPlayerState;
enum class EGameResult : uint8;
enum class EGamePhase : uint8;
struct FCoinLeaderboardEntry;

/**
 * 게임 결과 변경 알림 델리게이트
 * - 서버: 게임 데이터 동기화 및 관련 상태 갱신
 * - 클라이언트: UI 갱신 등 후속 처리
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnGameResultUpdated,
	const ACRPlayerState*,
	EGameResult); // NewGameResult

/**
 * 코인 변경 알림 델리게이트
 * - 서버: 게임 데이터 동기화 및 관련 상태 갱신
 * - 클라이언트: UI 갱신 등 후속 처리
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnCoinsUpdated,
	const ACRPlayerState*,
	int32); // NewCoins

/**
 * 보너스 최대 속도 변경 알림 델리게이트
 * - 서버: 게임 데이터 동기화 및 관련 상태 갱신
 * - 클라이언트: UI 갱신 등 후속 처리
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnBonusMaxSpeedUpdated,
	const ACRPlayerState*,
	float); // NewBonusMaxSpeed

/**
 *  Game Phase 변경 알림 델리게이트
 * - 서버: 게임 데이터 동기화 및 관련 상태 갱신
 * - 클라이언트: UI 갱신 등 후속 처리
 */
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnGamePhaseUpdated,
	EGamePhase); // NewGamePhase

/**
 *  남은 대기 시간 변경 알림 델리게이트
 * - 서버: 게임 데이터 동기화 및 관련 상태 갱신
 * - 클라이언트: UI 갱신 등 후속 처리
 */
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnWaitingTimeRemainingUpdated,
	float); // NewRemainingTime

/**
 *  남은 게임 시간 변경 알림 델리게이트
 * - 서버: 게임 데이터 동기화 및 관련 상태 갱신
 * - 클라이언트: UI 갱신 등 후속 처리
 */
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnPlayingTimeRemainingUpdated,
	float); // NewRemainingTime

/**
 *  우승 플레이어 변경 알림 델리게이트
 * - 서버: 게임 데이터 동기화 및 관련 상태 갱신
 * - 클라이언트: UI 갱신 등 후속 처리
 */
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnWinnerPlayerIdsUpdated,
	const TArray<int32>&); // NewWinners

/**
 * 코인 리더보드 변경 알림 델리게이트
 * - 서버: 게임 데이터 동기화 및 관련 상태 갱신
 * - 클라이언트: UI 갱신 등 후속 처리
 */
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnCoinLeaderboardUpdated,
	const TArray<FCoinLeaderboardEntry>&); // CoinLeaderboard

UCLASS()
class COINRACE_API UGameEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

  public:
	// 게임 결과 변경 알림 델리게이트
	FOnGameResultUpdated OnGameResultUpdated;

	// 코인 변경 알림 델리게이트
	FOnCoinsUpdated OnCoinsUpdated;

	// 보너스 최대 속도 변경 알림 델리게이트
	FOnBonusMaxSpeedUpdated OnBonusMaxSpeedUpdated;

	// Game Phase 변경 알림 델리게이트
	FOnGamePhaseUpdated OnGamePhaseUpdated;

	// 남은 대기 시간 변경 알림 델리게이트
	FOnWaitingTimeRemainingUpdated OnWaitingTimeRemainingUpdated;

	// 남은 게임 시간 변경 알림 델리게이트
	FOnPlayingTimeRemainingUpdated OnPlayingTimeRemainingUpdated;

	// 우승 플레이어 변경 알림 델리게이트
	FOnWinnerPlayerIdsUpdated OnWinnerPlayerIdsUpdated;

	// 코인 리더보드 변경 알림 델리게이트
	FOnCoinLeaderboardUpdated OnCoinLeaderboardUpdated;
};
