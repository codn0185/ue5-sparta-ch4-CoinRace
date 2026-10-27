#include "SkillWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Systems/GameEventSubsystem.h"

void USkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// OnSkillStateUpdated 델리게이트 바인딩
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnSkillStateUpdated.AddUObject(this, &ThisClass::HandleSkillStateUpdated);
		}
	}

	// 이미지에 Material 등록
	if (IsValid(CooldownMaterial) == true)
	{
		// 동적 Material Instance 생성
		DynamicCooldownMaterial = UMaterialInstanceDynamic::Create(CooldownMaterial, this);

		if (IsValid(DynamicCooldownMaterial) == true)
		{
			// 색상 설정
			DynamicCooldownMaterial->SetVectorParameterValue(
				TEXT("Color"),
				CooldownColor);

			// 이미지 등록
			Image_SkillCooldown->SetBrushFromMaterial(DynamicCooldownMaterial);
		}
	}

	// 초기 설정
	UpdateCooldown(0.f, 1.f);
	SetSkillActive(true);
	TextBlock_SkillName->SetText(FText::FromString(SkillName));
}

void USkillWidget::NativeDestruct()
{
	// OnSkillStateUpdated 델리게이트 바인딩 해제
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnSkillStateUpdated.RemoveAll(this);
		}
	}

	Super::NativeDestruct();
}

void USkillWidget::UpdateCooldown(float CooldownRemainingTime, float CooldownDuration)
{
	// 쿨다운 비율
	const float CooldownRatio = FMath::Clamp(
		CooldownRemainingTime / CooldownDuration,
		0.f,
		1.f);

	// 진행률
	const float Percentage = 1.f - CooldownRatio;

	// 진행률 설정
	if (IsValid(DynamicCooldownMaterial) == true)
	{
		DynamicCooldownMaterial->SetScalarParameterValue(
			TEXT("Percentage"),
			Percentage);
	}

	// 남은 쿨다운
	const float DisplayTime = FMath::Max(0.0f, CooldownRemainingTime);

	// 소수점 첫째 자리까지 표시
	FNumberFormattingOptions NumberFormat;
	NumberFormat.SetMinimumFractionalDigits(1);
	NumberFormat.SetMaximumFractionalDigits(1);

	// 남은 시간 텍스트 UI 설정
	TextBlock_RemainingTime->SetText(FText::AsNumber(DisplayTime, &NumberFormat));
}

void USkillWidget::SetSkillActive(bool bIsActive)
{
	if (bIsActive == true)
	{
		TextBlock_RemainingTime->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TextBlock_RemainingTime->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillWidget::HandleSkillStateUpdated(const FSkillState& SkillState)
{
	if (SkillType != SkillState.SkillType)
	{
		return;
	}

	UpdateCooldown(SkillState.CooldownRemainingTime, SkillState.CooldownDuration);

	SetSkillActive(SkillState.bCanUseSkill);
}
