// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
/**
 * 
 */
UCLASS()
class TESTPROJECT_AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();
	
	// 添加角色初始拥有的 GA
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

	// 按住指定按键时激活 GA
	void AbilityInputTagHeld(const FGameplayTag& Tags);
	// 松开指定按键时不会激活 GA ，但也需要通知
	void AbilityInputTagReleased(const FGameplayTag& Tags);

	// RPC 函数，绑定委托时使客户端也能够调用此函数
	UFUNCTION(Client, Reliable)
	void ClientOnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle) const;


	FEffectAssetTags EffectAssetTags;

};
