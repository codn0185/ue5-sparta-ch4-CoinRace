#include "CRGameState.h"

#include "Net/UnrealNetwork.h"

void ACRGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, MaxChatLogCount);
}

void ACRGameState::AddChatData(const FChatData& NewChatData)
{
	if (ChatLog.Num() >= MaxChatLogCount)
	{
		ChatLog.RemoveAt(0);
	}

	ChatLog.Add(NewChatData);
}

void ACRGameState::SetChatLog(const TArray<FChatData>& InChatLog)
{
	ChatLog = InChatLog;
}

void ACRGameState::ClearChatLog()
{
	ChatLog.Empty();
}

const TArray<FChatData>& ACRGameState::GetChatLog() const
{
	return ChatLog;
}
