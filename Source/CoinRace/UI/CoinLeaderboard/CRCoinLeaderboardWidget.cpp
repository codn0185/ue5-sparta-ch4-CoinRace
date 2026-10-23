#include "CRCoinLeaderboardWidget.h"

#include "Components/ListView.h"
#include "Frameworks/CRGameState.h"
#include "GameFramework/PlayerState.h"
#include "Systems/GameEventSubsystem.h"
#include "UI/CoinLeaderboard/CRCoinLeaderboardItem.h"

void UCRCoinLeaderboardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// OnCoinLeaderboardUpdated 델리게이트 바인딩
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			CoinLeaderboardUpdatedHandle = GameEventSubsystem->OnCoinLeaderboardUpdated.AddUObject(this, &ThisClass::HandleCoinLeaderboardUpdated);
		}
	}

	// 초기 CoinLeaderboard 설정
	ACRGameState* CRGM = GetWorld()->GetGameState<ACRGameState>();
	if (IsValid(CRGM) == true)
	{
		UpdateCoinLeaderboard(CRGM->GetCoinLeaderboard());
	}
	else
	{
		ClearCoinLeaderboard();
	}
}

void UCRCoinLeaderboardWidget::NativeDestruct()
{
	// OnCoinLeaderboardUpdated 델리게이트 바인딩 제거
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnCoinLeaderboardUpdated.Remove(CoinLeaderboardUpdatedHandle);
			CoinLeaderboardUpdatedHandle.Reset();
		}
	}

	Super::NativeDestruct();
}

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

void UCRCoinLeaderboardWidget::HandleCoinLeaderboardUpdated(const TArray<FCoinLeaderboardEntry>& CoinLeaderboard)
{
	UpdateCoinLeaderboard(CoinLeaderboard);
}
