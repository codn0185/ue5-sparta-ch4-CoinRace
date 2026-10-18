#include "CRHUD.h"

#include "CRVehicleStatusWidget.h"

#include "UI/CRBonusMaxSpeedWidget.h"
#include "UI/CRCoinWidget.h"
#include "UI/Chat/CRChatWidget.h"
#include "UI/CoinLeaderboard/CRCoinLeaderboardWidget.h"

void UCRHUD::NativeConstruct()
{
	Super::NativeConstruct();

	// 채팅 입력 커밋 델리게이트 바인딩
	ChatWidget->OnChatMessageSubmitted.BindLambda(
		[this](const FString& Message)
		{
			OnChatMessageSubmitted.ExecuteIfBound(Message);
		});
}

void UCRHUD::SetSpeedUnit(const FSpeedUnit& NewSpeedUnit)
{
	VehicleStatusWidget->SetSpeedUnit(NewSpeedUnit);
	BonusMaxSpeedWidget->SetSpeedUnit(NewSpeedUnit);
}

void UCRHUD::UpdateSpeed(float NewSpeed)
{
	VehicleStatusWidget->UpdateSpeed(NewSpeed);
}

void UCRHUD::UpdateGear(int32 NewGear)
{
	VehicleStatusWidget->UpdateGear(NewGear);
}

void UCRHUD::AddChatMessage(const FChatData& ChatData)
{
	ChatWidget->AddChatMessage(ChatData);
}

void UCRHUD::ClearChatMessages()
{
	ChatWidget->ClearChatMessages();
}

void UCRHUD::FocusChatInput()
{
	ChatWidget->FocusChatInput();
}

void UCRHUD::SetCoins(int32 Amount)
{
	CoinWidget->SetCoins(Amount);
}

void UCRHUD::SetBonusMaxSpeed(float NewBonusMaxSpeed)
{
	BonusMaxSpeedWidget->SetBonusMaxSpeed(NewBonusMaxSpeed);

	// 0 이하면 숨기기
	BonusMaxSpeedWidget->SetVisibility(
		NewBonusMaxSpeed > 0.f
			? ESlateVisibility::Visible
			: ESlateVisibility::Hidden);
}

void UCRHUD::UpdateCoinLeaderboard(const TArray<FCoinLeaderboardEntry>& CoinLeaderboard)
{
	CoinLeaderboardWidget->UpdateCoinLeaderboard(CoinLeaderboard);
}

void UCRHUD::ClearCoinLeaderboard()
{
	CoinLeaderboardWidget->ClearCoinLeaderboard();
}
