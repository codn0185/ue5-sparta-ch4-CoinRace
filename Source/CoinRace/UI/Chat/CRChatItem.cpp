#include "CRChatItem.h"

FChatEntry UCRChatItem::GetChatData() const
{
	return ChatData;
}

void UCRChatItem::SetChatData(const FChatEntry& InChatData)
{
	ChatData = InChatData;
}
