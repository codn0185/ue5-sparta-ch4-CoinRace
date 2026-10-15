#include "ChatWorldSubsystem.h"

void UChatWorldSubsystem::AddChatData(const FChatData& NewChatData)
{
	if (ChatLog.Num() >= MaxChatLogCount)
	{
		ChatLog.RemoveAt(0);
	}

	ChatLog.Add(NewChatData);
}

const TArray<FChatData>& UChatWorldSubsystem::GetChatLog() const
{
	return ChatLog;
}
