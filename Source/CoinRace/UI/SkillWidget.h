#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "SkillWidget.generated.h"

class UImage;
class UTextBlock;
class UMaterialInterface;
class UMaterialInstanceDynamic;
enum class ESkillType : uint8;
struct FSkillState;

UCLASS()
class COINRACE_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_SkillCooldown;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_RemainingTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SkillName;

	// 원본 쿨다운 머티리얼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill)
	TObjectPtr<UMaterialInterface> CooldownMaterial;

	// 동적 쿨다운 머티리얼
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicCooldownMaterial;

	// 쿨다운 색상
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill)
	FLinearColor CooldownColor = FLinearColor::White;

  protected:
	// 스킬 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill)
	ESkillType SkillType = ESkillType();

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

  protected:
	void UpdateCooldown(float CooldownRemainingTime, float CooldownDuration);

	void SetSkillActive(bool bIsActive);

  protected:
	UFUNCTION()
	void HandleSkillStateUpdated(const FSkillState& SkillState);
};
