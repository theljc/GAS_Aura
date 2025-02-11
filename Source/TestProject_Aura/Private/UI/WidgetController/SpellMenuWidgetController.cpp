// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BindCallBacksDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.AbilityStatus = StatusTag;
			bool bSpellPointsButtonEnable = false;
			bool bEquippedButtonEnable = false;
			ShouldButtonEnable(StatusTag, CurrentSpellPoints, bSpellPointsButtonEnable, bEquippedButtonEnable);
			FString Description;
			FString NextLevelDescription;
			GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
			SpellGlobleSelectedDelegate.Broadcast(bSpellPointsButtonEnable, bEquippedButtonEnable, Description, NextLevelDescription);
		}
		
		if (AbilityInfo)
		{
			FAuraAbilityInfo AuraAbilityInfo = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			AuraAbilityInfo.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(AuraAbilityInfo);
		}
	});

	GetAuraPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 SpellPoints)
	{
		OnSpellPointsChanged.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;
		bool bSpellPointsButtonEnable = false;
		bool bEquippedButtonEnable = false;
		ShouldButtonEnable(SelectedAbility.AbilityStatus, CurrentSpellPoints, bSpellPointsButtonEnable, bEquippedButtonEnable);
		FString Description;
		FString NextLevelDescription;
		GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectedAbility.AbilityTag, Description, NextLevelDescription);
		SpellGlobleSelectedDelegate.Broadcast(bSpellPointsButtonEnable, bEquippedButtonEnable, Description, NextLevelDescription);
	});
	
}

void USpellMenuWidgetController::BroadCastInitialValues()
{
	BroadcastAbilityInfo();

	OnSpellPointsChanged.Broadcast(GetAuraPlayerState()->GetPlayerSpellPoints());
	
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->ServerSpendSpellPoints(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::SpellGlobleSelected(const FGameplayTag& AbilityTag)
{
	FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPlayerState()->GetPlayerSpellPoints();
	FGameplayTag AbilityStatus;
	
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(AuraGameplayTags.Abilities_Type_None);
	FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = AuraGameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.AbilityStatus = AbilityStatus;
	bool bSpellPointsButtonEnable = false;
	bool bEquippedButtonEnable = false;
	ShouldButtonEnable(AbilityStatus, SpellPoints, bSpellPointsButtonEnable, bEquippedButtonEnable);
	FString Description;
	FString NextLevelDescription;
	GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobleSelectedDelegate.Broadcast(bSpellPointsButtonEnable, bEquippedButtonEnable, Description, NextLevelDescription);	
}

void USpellMenuWidgetController::ShouldButtonEnable(const FGameplayTag& AbilityStatus, const int32 SpellPoints,
	bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquippedButton)
{
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquippedButton = false;
	
	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	if (AbilityStatus == AuraGameplayTags.Abilities_Status_Equipped)
	{
		bShouldEnableEquippedButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus == AuraGameplayTags.Abilities_Status_Eligible)
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus == AuraGameplayTags.Abilities_Status_Unlocked)
	{
		bShouldEnableEquippedButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	
}
