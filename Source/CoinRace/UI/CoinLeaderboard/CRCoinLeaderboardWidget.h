#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Data/CoinLeaderboardEntry.h"

#include "CRCoinLeaderboardWidget.generated.h"

struct FCoinLeaderboardEntry;

UCLASS()
class COINRACE_API UCRCoinLeaderboardWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	// 코인 리더보드
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> ListView_CoinLeaderboard;

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

  public:
	// 코인 리더보드 업데이트
	void UpdateCoinLeaderboard(const TArray<FCoinLeaderboardEntry>& CoinLeaderboard);

	// 코인 리더보드 초기화
	void ClearCoinLeaderboard();

	/** OnCoinLeaderboardUpdated 델리게이트 관리 */
  protected:
	FDelegateHandle CoinLeaderboardUpdatedHandle;

	UFUNCTION()
	void HandleCoinLeaderboardUpdated(const TArray<FCoinLeaderboardEntry>& CoinLeaderboard);
};
