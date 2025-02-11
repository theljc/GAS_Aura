// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

struct FAuraGameplayTags;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobleSelectedSignature, bool, bSpellPointsButtonEnable, bool,
                                             bEquippedButtonEnable, FString, Description, FString, NextLevelDescription);

USTRUCT()
struct FSelectedAbility
{
	GENERATED_BODY()
	FGameplayTag AbilityTag;
	FGameplayTag AbilityStatus;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TESTPROJECT_AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallBacksDependencies() override;
	virtual void BroadCastInitialValues() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature OnSpellPointsChanged;

	UPROPERTY(BlueprintAssignable)
	FSpellGlobleSelectedSignature SpellGlobleSelectedDelegate;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobleSelected(const FGameplayTag& AbilityTag);
	
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();
	
private:
	static void ShouldButtonEnable(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquippedButton);
	FSelectedAbility SelectedAbility = {FAuraGameplayTags::Get().Abilities_Type_None, FAuraGameplayTags::Get().Abilities_Status_Locked};
	int32 CurrentSpellPoints = 0;
};
