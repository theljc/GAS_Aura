// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCoolDownChange.h"

#include "AbilitySystemComponent.h"

UWaitCoolDownChange* UWaitCoolDownChange::WaitForCoolDownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                                const FGameplayTag& InCoolDownTag)
{
	UWaitCoolDownChange* WaitCoolDownChange = NewObject<UWaitCoolDownChange>();
	WaitCoolDownChange->ASC = AbilitySystemComponent;
	WaitCoolDownChange->CoolDownTag = InCoolDownTag;

	if (!IsValid(AbilitySystemComponent) || !InCoolDownTag.IsValid())
	{
		WaitCoolDownChange->EndTask();
		return nullptr;
	}

	// 何时结束了冷却（通过 CoolDown Tag 被移除来判断）
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InCoolDownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
		WaitCoolDownChange, &UWaitCoolDownChange::CoolDownChanged);

	// 何时应用了冷却
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		WaitCoolDownChange, &UWaitCoolDownChange::OnActiveEffectAdded);

	return WaitCoolDownChange;
}

void UWaitCoolDownChange::EndTask()
{
	if (!IsValid(ASC))
	{
		return;
	}
	
	ASC->RegisterGameplayTagEvent(
		CoolDownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCoolDownChange::CoolDownChanged(const FGameplayTag InCoolDownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CoolDownEnd.Broadcast(0.f);
	}
	
}

void UWaitCoolDownChange::OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);
	
	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CoolDownTag) || GrantedTags.HasTagExact(CoolDownTag))
	{
		FGameplayEffectQuery GameplayTagQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CoolDownTag.GetSingleTagContainer());
		TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(GameplayTagQuery);
		if (TimesRemaining.Num() > 0)
		{
			float TimeRemaining = TimesRemaining[0];
			for (int32 i = 0; i < TimesRemaining.Num(); i++)
			{
				if (TimeRemaining < TimesRemaining[i])
				{
					TimeRemaining = TimesRemaining[i];
				}
			}

			CoolDownStart.Broadcast(TimeRemaining);
		}
	}
	
}
