// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "TestProject_Aura/AuraLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientOnEffectApplied);
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	// GameplayTags.Attribute_Secondary_Armor.ToString();
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *GameplayTags.Attribute_Secondary_Armor.ToString()));
}

void UAuraAbilitySystemComponent::ClientOnEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                  const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) const
{
	// 在 GE 应用的时候，获得 Tag 的信息保存到 TagContainer
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);
	// 广播委托，传入保存的 Tag 信息
	EffectAssetTags.Broadcast(TagContainer);
	
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	// 遍历数组，激活 GA
	for (const TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->StartUpInputTag);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
	// 遍历数组，激活 GA (被动技能)
	for (const TSubclassOf<UGameplayAbility> AbilityClass : PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
	
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& Tags)
{
	if (!Tags.IsValid())
	{
		return;
	}
	
	for (auto& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tags))
		{
			// 表示有按键输入
			AbilitySpecInputPressed(AbilitySpec);
			// 如果未激活状态则激活 GA
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}

}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& Tags)
{
	if (!Tags.IsValid())
	{
		return;
	}
	
	for (auto& AbilitySpec:GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tags))
		{
			// 表示有按键输入
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& Spec)
{
	if (Spec.Ability)
	{
		for (FGameplayTag Tag : Spec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& Spec)
{
	for (FGameplayTag Tag : Spec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::ForEachAbilities(const FForEachAbilities& Delegate)
{
	// 在循环遍历时锁定数组中的数据，防止遍历时被修改
	FScopedAbilityListLock ActiveScopedLock(*this);
	
	for (const FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			// %hs 输出函数名
			UE_LOG(LogAura, Error, TEXT("Failed To Executed Function %hs"), __FUNCTION__);
		}
	}
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}
	
}
