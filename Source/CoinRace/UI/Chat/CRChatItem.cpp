#include "CRChatItem.h"

FChatData UCRChatItem::GetChatData() const
{
	return ChatData;
}

void UCRChatItem::SetChatData(const FChatData& InChatData)
{
	ChatData = InChatData;
}
