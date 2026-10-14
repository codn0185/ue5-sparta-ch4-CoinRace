#pragma once

#include "CRChatData.h"
#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"

#include "CRChatItem.generated.h"

UCLASS()
class COINRACE_API UCRChatItem : public UObject
{
	GENERATED_BODY()

  protected:
	FCRChatData ChatData;

  public:
	FCRChatData GetChatData() const;

	void SetChatData(const FCRChatData InChatData);
};
