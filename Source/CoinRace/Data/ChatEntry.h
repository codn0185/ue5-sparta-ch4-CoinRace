#pragma once

#include "CoreMinimal.h"

#include "ChatEntry.generated.h"

USTRUCT()
struct FChatEntry
{
	GENERATED_BODY()

  public:
	// 송신자 이름
	UPROPERTY()
	FString SenderNameString = TEXT("None");
	// 송신자 이름 색상
	UPROPERTY()
	FLinearColor SenderNameColor = FLinearColor::Gray;

	// 채팅 메시지
	UPROPERTY()
	FString ChatMessageString;
	// 채팅 메시지 색상
	UPROPERTY()
	FLinearColor ChatMessageColor = FLinearColor::White;
};
