#include "ChatWorldSubsystem.h"

void UChatWorldSubsystem::AddChatData(const FChatData& NewChatData)
{
	if (ChatLog.Num() >= MaxChatLogCount)
	{
		ChatLog.RemoveAt(0);
	}

	ChatLog.Add(NewChatData);
}

void UChatWorldSubsystem::SetChatLog(const TArray<FChatData>& InChatLog)
{
	ChatLog = InChatLog;
}

void UChatWorldSubsystem::ClearChatLog()
{
	ChatLog.Empty();
}

const TArray<FChatData>& UChatWorldSubsystem::GetChatLog() const
{
	return ChatLog;
}
