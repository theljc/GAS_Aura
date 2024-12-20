// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraDataAsset.h"
#include "Misc/LowLevelTestAdapter.h"

void UAttributeWidgetController::BindCallBacksDependencies()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	// 遍历 Map,设置 AttributeValue 属性的值并广播
	for (auto& Pair: AS->TagsToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
			{
				// Pair.Value 是函数指针
				BroadcastAttributeValues(Pair.Key, Pair.Value());
			}
		);
	}
	
}

void UAttributeWidgetController::BroadCastInitialValues()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	// 遍历 Map,设置 AttributeValue 属性的值并广播
	for (auto& Pair: AS->TagsToAttribute)
	{
		// Pair.Value 是函数指针
		BroadcastAttributeValues(Pair.Key, Pair.Value());
	}

}

void UAttributeWidgetController::BroadcastAttributeValues(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, AttributeTag.GetTagName().ToString());
	
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
