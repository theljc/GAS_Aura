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

	GetAuraAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

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

void USpellMenuWidgetController::GlobleDeSelected()
{
	if (bWaitForSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitForSelection = false;
	}
	
	SelectedAbility.AbilityTag = FAuraGameplayTags::Get().Abilities_Type_None;
	SelectedAbility.AbilityStatus = FAuraGameplayTags::Get().Abilities_Status_Locked;
}

void USpellMenuWidgetController::EquippedButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
	WaitForSelectionDelegate.Broadcast(AbilityType);
	bWaitForSelection = true;

	FGameplayTag SelectedTag = GetAuraAbilitySystemComponent()->GetStatusFromAbilityTag(SelectedAbility.AbilityTag);
	if (SelectedTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraAbilitySystemComponent()->GetSlotFromAbilityTag(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::SpellGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitForSelection) return;

	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetAuraAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.AbilityTag, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bWaitForSelection = false;

	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = AuraGameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = AuraGameplayTags.Abilities_Type_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SpellGlobeResignedDelegate.Broadcast(AbilityTag);
	GlobleDeSelected();
	
}

void USpellMenuWidgetController::SpellGlobleSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitForSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitForSelection = false;
	}
	
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
