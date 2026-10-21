#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "CRGameState.generated.h"

struct FChatEntry;

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
	TArray<FChatEntry> ChatLog;

  public:
	// 채팅 추가
	void AddChatData(const FChatEntry& NewChatData);

	// 채팅 로그 설정
	void SetChatLog(const TArray<FChatEntry>& InChatLog);

	// 채팅 로그 초기화
	void ClearChatLog();

	// 채팅 로그 반환
	const TArray<FChatEntry>& GetChatLog() const;
};
