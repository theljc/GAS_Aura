// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BindCallBacksDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
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
	});
	
}

void USpellMenuWidgetController::BroadCastInitialValues()
{
	BroadcastAbilityInfo();

	OnSpellPointsChanged.Broadcast(GetAuraPlayerState()->GetPlayerSpellPoints());
	
}
