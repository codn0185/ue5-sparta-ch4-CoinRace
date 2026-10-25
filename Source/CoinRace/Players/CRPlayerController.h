#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "CRPlayerController.generated.h"

class UInputMappingContext;
class ACRWheeledVehiclePawn;
class UCRHUD;
class UCRLobbyWidget;
class UCRGameResultWidget;
struct FChatEntry;
struct FCoinLeaderboardEntry;

UCLASS()
class COINRACE_API ACRPlayerController : public APlayerController
{
	GENERATED_BODY()

  protected:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	/** If true, the optional steering wheel input mapping context will be registered */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	bool bUseSteeringWheelControls = false;

	/** Optional Input Mapping Context to be used for steering wheel input.
	 *  This is added alongside the default Input Mapping Context and does not block other forms of input.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (EditCondition = "bUseSteeringWheelControls"))
	UInputMappingContext* SteeringWheelInputMappingContext;

	/** Pointer to the controlled vehicle pawn */
	UPROPERTY(Replicated)
	TObjectPtr<ACRWheeledVehiclePawn> VehiclePawn;

	/** Type of the UI to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UCRHUD> VehicleUIClass;

	/** Pointer to the UI widget */
	TObjectPtr<UCRHUD> VehicleUI;

	/** 대기실 UI 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UCRLobbyWidget> LobbyUIClass;

	/** 대기실 UI 위젯 */
	TObjectPtr<UCRLobbyWidget> LobbyUI;

	/** 게임 결과 UI 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UCRGameResultWidget> GameResultUIClass;

	/** 대기실 UI 위젯 */
	TObjectPtr<UCRGameResultWidget> GameResultUI;

	// GamePhase 변경 이벤트
	UFUNCTION()
	void HandleGamePhaseUpdated(EGamePhase NewGamePhase);

	// RPC 메서드
  public:
	// 서버에 채팅 메시지 전송을 요청
	UFUNCTION(Server, Reliable)
	void ServerRPCSendChatMessage(const FString& ChatMessage);
	// 서버로부터 받은 채팅 메시지를 UI에 추가
	UFUNCTION(Client, Reliable)
	void ClientRPCReciveChatMessage(const FChatEntry& ChatData);
	// 서버로부터 채팅 로그를 받아 동기화
	UFUNCTION(Client, Reliable)
	void ClientRPCSyncChatLog(const TArray<FChatEntry>& ChatLog);

  protected:
	// BeginPlay() 직후 호출되어, 플레이어가 준비됨을 서버에 알림
	UFUNCTION(Server, Reliable)
	void ServerRPCNotifyPlayerInitialized();

	// Begin Actor interface
  protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

  public:
	virtual void Tick(float Delta) override;

	// End Actor interface

	// Begin PlayerController interface
  protected:
	virtual void OnPossess(APawn* InPawn) override;

	// End PlayerController interface

  public:
	void FocusChatInput();
};
