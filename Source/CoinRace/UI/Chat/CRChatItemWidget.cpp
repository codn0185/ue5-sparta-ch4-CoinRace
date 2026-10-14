#include "CRChatItemWidget.h"

#include "CRChatData.h"
#include "CRChatItem.h"

#include "Components/TextBlock.h"

void UCRChatItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// UCRChatItem 캐스팅
	UCRChatItem* ChatItem = Cast<UCRChatItem>(ListItemObject);

	if (IsValid(ChatItem) == false)
	{
		return;
	}

	// 채팅 데이터
	const FCRChatData ChatData = ChatItem->GetChatData();

	// TextBlock_SenderName 위젯 설정
	TextBlock_SenderName->SetText(FText::FromString(ChatData.SenderNameString));
	TextBlock_SenderName->SetColorAndOpacity(ChatData.SenderNameColor);
	// TextBlock_ChatMessage 위젯 설정
	TextBlock_ChatMessage->SetText(FText::FromString(ChatData.ChatMessageString));
	TextBlock_ChatMessage->SetColorAndOpacity(ChatData.ChatMessageColor);
}
