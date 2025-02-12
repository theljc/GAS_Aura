// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
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

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool InBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit (UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category = "AbilitySystemLibrary|GameplayMechanics")
	static void GetLiveActorsWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* SecondActor);
	
	UFUNCTION(BlueprintPure, Category = "AbilitySystemLibrary|XP")
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
	
};
