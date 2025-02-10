// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraDataAsset.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Player/AuraPlayerState.h"

void UAttributeWidgetController::BindCallBacksDependencies()
{
	check(AttributeInfo);

	// 遍历 Map,设置 AttributeValue 属性的值并广播
	for (auto& Pair: GetAuraAttributeSet()->TagsToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
			{
				// Pair.Value 是函数指针
				BroadcastAttributeValues(Pair.Key, Pair.Value());
			}
		);
	}

	GetAuraPlayerState()->OnAttributePointsChangedDelegate.AddLambda(
		[this](const int32 AttributePoints)
		{
			AttributePointsChangedDelegate.Broadcast(AttributePoints);
		}
	);
	
}

void UAttributeWidgetController::BroadCastInitialValues()
{
	check(AttributeInfo);

	// 遍历 Map,设置 AttributeValue 属性的值并广播
	for (auto& Pair: GetAuraAttributeSet()->TagsToAttribute)
	{
		// Pair.Value 是函数指针
		BroadcastAttributeValues(Pair.Key, Pair.Value());
	}

	AttributePointsChangedDelegate.Broadcast(GetAuraPlayerState()->GetPlayerAttributePoints());
	
}

void UAttributeWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}

void UAttributeWidgetController::BroadcastAttributeValues(const FGameplayTag& AttributeTag,
                                                          const FGameplayAttribute& Attribute) const
{
	
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
