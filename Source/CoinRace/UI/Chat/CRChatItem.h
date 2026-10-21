#pragma once

#include "CoreMinimal.h"

#include "Data/ChatEntry.h"
#include "UObject/NoExportTypes.h"

#include "CRChatItem.generated.h"

UCLASS()
class COINRACE_API UCRChatItem : public UObject
{
	GENERATED_BODY()

  protected:
	FChatEntry ChatData;

  public:
	FChatEntry GetChatData() const;

	void SetChatData(const FChatEntry& InChatData);
};
