#pragma once

#include "CoreMinimal.h"

#include "Frameworks/CRGameState.h"
#include "UObject/NoExportTypes.h"

#include "CRChatItem.generated.h"

UCLASS()
class COINRACE_API UCRChatItem : public UObject
{
	GENERATED_BODY()

  protected:
	FChatData ChatData;

  public:
	FChatData GetChatData() const;

	void SetChatData(const FChatData& InChatData);
};
