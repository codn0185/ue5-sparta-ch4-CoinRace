#include "CRChatItemWidget.h"

#include "Components/TextBlock.h"
#include "Data/ChatEntry.h"
#include "UI/Chat/CRChatItem.h"

void UCRChatItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// UCRChatItem 캐스팅
	UCRChatItem* ChatItem = Cast<UCRChatItem>(ListItemObject);

	if (IsValid(ChatItem) == false)
	{
		return;
	}

	// 채팅 데이터
	const FChatEntry ChatData = ChatItem->GetChatData();

	// TextBlock_SenderName 위젯 설정
	TextBlock_SenderName->SetText(FText::FromString(ChatData.SenderNameString));
	TextBlock_SenderName->SetColorAndOpacity(ChatData.SenderNameColor);
	// TextBlock_ChatMessage 위젯 설정
	TextBlock_ChatMessage->SetText(FText::FromString(ChatData.ChatMessageString));
	TextBlock_ChatMessage->SetColorAndOpacity(ChatData.ChatMessageColor);
}
