// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayEffectType.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilityInfo;
class AAuraHUD;
struct FAuraWidgetControllerParam;
class USpellMenuWidgetController;
class UAttributeWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class TESTPROJECT_AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController", meta = (DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParam(const UObject* WorldContextObject, FAuraWidgetControllerParam& OutWidgetControllerParam, AAuraHUD*& OutHUD);
	
	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController", meta = (DefaultToSelf="WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController", meta = (DefaultToSelf="WorldContextObject"))
	static UAttributeWidgetController* GetAttributeWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|WidgetController", meta = (DefaultToSelf="WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	// 静态函数，初始化默认属性
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassDefaults")
	static void InitalizeDefaultAttribute(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartUpAbility(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|CharacterClassDefaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit (const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit (const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff (const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage (const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration (const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency (const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType (const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpluse (const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayEffects")
	static FVector GetKnockbackForce (const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);


	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool InBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetDamageType (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpluse (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpluse);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackForce (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin);

	
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayMechanics")
	static void GetLiveActorsWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	// 从 Actors 中获得距离 Origin 位置最近的 MaxTargets 个 Actor，保存在 OutClosestTargets 中
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);
	
	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParam& DamageEffectParam);


	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotator);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVector);
	
	// UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|XP")
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
	
};
