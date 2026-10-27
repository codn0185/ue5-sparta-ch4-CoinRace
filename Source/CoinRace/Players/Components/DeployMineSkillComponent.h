#pragma once

#include "CoreMinimal.h"

#include "Players/Components/SkillComponentBase.h"

#include "DeployMineSkillComponent.generated.h"

class AMineItem;

UCLASS()
class COINRACE_API UDeployMineSkillComponent : public USkillComponentBase
{
	GENERATED_BODY()

  protected:
	// 지뢰 아이템 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TSubclassOf<AMineItem> MineItemClass;

  public:
	UDeployMineSkillComponent();

  protected:
	virtual void ActivateSkill() override;
};
