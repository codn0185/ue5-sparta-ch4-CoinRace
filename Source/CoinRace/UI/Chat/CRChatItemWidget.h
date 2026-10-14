#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"

#include "CRChatItemWidget.generated.h"

class UTextBlock;

UCLASS()
class COINRACE_API UCRChatItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

  protected:
	// 송신자 이름
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SenderName;

	// 메시지 내용
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ChatMessage;

  protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
