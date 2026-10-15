#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CRChatWidget.generated.h"

class UListView;
class UEditableTextBox;

struct FCRChatData;

// 채팅 입력 커밋 시 메시지를 송출하는 델리게이트
DECLARE_DELEGATE_OneParam(
	FOnChatMessageSubmitted,
	const FString&);

UCLASS()
class COINRACE_API UCRChatWidget : public UUserWidget
{
	GENERATED_BODY()

  public:
	// 채팅 입력 커밋 시 실행되는 델리게이트
	FOnChatMessageSubmitted OnChatMessageSubmitted;

  protected:
	// 채팅 기록
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> ListView_ChatLog;

	// 채팅 입력
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

  protected:
	virtual void NativeConstruct() override;

  public:
	// 채팅 추가
	void AddChatMessage(const FCRChatData& ChatData);

	// 채팅 클리어
	void ClearChatMessages();

	// 채팅 입력창 포커스
	void FocusChatInput();

  protected:
	// 채팅 입력 커밋 시 호출
	UFUNCTION()
	void OnChatInputCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
