#include "CRChatItem.h"

FCRChatData UCRChatItem::GetChatData() const
{
	return ChatData;
}

void UCRChatItem::SetChatData(const FCRChatData InChatData)
{
	ChatData = InChatData;
}
