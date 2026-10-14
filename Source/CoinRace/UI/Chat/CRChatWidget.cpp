#include "CRChatWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "UI/Chat/CRChatData.h"
#include "UI/Chat/CRChatItem.h"

void UCRChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 채팅 입력 커밋 이벤트 바인딩
	EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(
		this,
		&ThisClass::OnChatInputCommitted);
}

void UCRChatWidget::AddChatMessage(const FCRChatData& ChatData)
{
	// 채팅 아이템 오브젝트 생성 및 설정
	UCRChatItem* ChatItem = NewObject<UCRChatItem>(this);
	ChatItem->SetChatData(ChatData);

	// 리스트뷰에 추가
	ListView_ChatLog->AddItem(ChatItem);
}

void UCRChatWidget::ClearChatMessages()
{
	ListView_ChatLog->ClearListItems();
}

void UCRChatWidget::OnChatInputCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 엔터 키 입력만 허용
	if (CommitMethod != ETextCommit::OnEnter)
	{
		return;
	}

	// FString 타입 전환 (뒷부분 공백 제거)
	const FString Message = Text.ToString().TrimEnd();

	// 빈 입력 무시
	if (Text.IsEmpty() == true)
	{
		return;
	}

	// 입력 텍스트 제거
	EditableTextBox_ChatInput->SetText(FText::GetEmpty());

	// 델리게이트 실행
	OnChatMessageSubmitted.ExecuteIfBound(Message);
}
