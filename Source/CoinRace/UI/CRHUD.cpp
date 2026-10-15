#include "CRHUD.h"

#include "CRVehicleStatusWidget.h"

#include "Chat/CRChatWidget.h"
#include "UI/CRBonusMaxSpeedWidget.h"
#include "UI/CRCoinWidget.h"

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
}
