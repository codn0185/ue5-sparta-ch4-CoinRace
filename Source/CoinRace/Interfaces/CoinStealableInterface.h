#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "CoinStealableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCoinStealableInterface : public UInterface
{
	GENERATED_BODY()
};

class COINRACE_API ICoinStealableInterface
{
	GENERATED_BODY()

  public:
	// 빼앗을 코인 수를 전달하고, 실제로 빼앗은 코인 수를 반환
	virtual int32 StealCoins(int32 Amount) = 0;
};
