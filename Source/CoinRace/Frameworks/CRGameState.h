#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "CRGameState.generated.h"

/** 채팅 데이터 */
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
class COINRACE_API ACRGameState : public AGameStateBase
{
	GENERATED_BODY()

	// 레플리케이션 대상 등록
  protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// ==================================================
	// 채팅 로그 관리
  protected:
	// 채팅 로그의 최대 보관 개수
	UPROPERTY(Replicated)
	int32 MaxChatLogCount = 20;

	// 채팅 로그
	TArray<FChatData> ChatLog;

  public:
	// 채팅 추가
	void AddChatData(const FChatData& NewChatData);

	// 채팅 로그 설정
	void SetChatLog(const TArray<FChatData>& InChatLog);

	// 채팅 로그 초기화
	void ClearChatLog();

	// 채팅 로그 반환
	const TArray<FChatData>& GetChatLog() const;
};
