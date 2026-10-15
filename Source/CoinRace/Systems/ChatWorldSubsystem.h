#pragma once

#include "CoreMinimal.h"

#include "Subsystems/WorldSubsystem.h"

#include "ChatWorldSubsystem.generated.h"

USTRUCT()
struct FChatData
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

UCLASS()
class COINRACE_API UChatWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

  protected:
	// 보관할 채팅 로그의 최대 개수
	static inline int32 MaxChatLogCount = 20;

	// 보관된 채팅 목록 (큐 방식)
	TArray<FChatData> ChatLog;

  public:
	// 채팅 추가
	void AddChatData(const FChatData& NewChatData);

	// 채팅 로그 반환
	const TArray<FChatData>& GetChatLog() const;
};
