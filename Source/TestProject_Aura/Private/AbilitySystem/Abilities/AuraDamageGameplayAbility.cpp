// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	const float ScaleDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, ScaleDamage);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}

FDamageEffectParam UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefault(AActor* TargetActor) const
{
	FDamageEffectParam Param;
	Param.WorldContextObject = GetAvatarActorFromActorInfo();
	Param.DamageGameplayEffectClass = DamageEffectClass;
	Param.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Param.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Param.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Param.AbilityLevel = GetAbilityLevel();
	Param.DamageType = DamageType;
	Param.DebuffDamage = DebuffDamage;
	Param.DebuffChance = DebuffChance;
	Param.DebuffDuration = DebuffDuration;
	Param.DebuffFrequency = DebuffFrequency;
	Param.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Param.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Param.KnockbackChance = KnockbackChance;
	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		Param.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Param.KnockbackForce = ToTarget * KnockbackForceMagnitude;
	}

	if (bIsRadialDamage)
	{
		Param.bIsRadialDamage = bIsRadialDamage;
		Param.RadialDamageOrigin = RadialDamageOrigin;
		Param.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Param.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	
	return Param;
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Index = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Index];
	}
	return FTaggedMontage();
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}
