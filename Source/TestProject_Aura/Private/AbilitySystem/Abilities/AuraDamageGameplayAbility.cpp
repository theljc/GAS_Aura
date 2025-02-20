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

FDamageEffectParam UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefault(AActor* TargetActor,
	FVector InRadialDamageOrigin, bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride,
	bool bOverrideDeathImpulse, FVector DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
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
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		const FVector ToTarget = Rotation.Vector();
		if (!bOverrideKnockbackDirection)
		{
			Param.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		}
		if (!bOverrideDeathImpulse)
		{
			Param.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		}
	}


	if (bOverrideKnockbackDirection)
	{
		KnockbackDirectionOverride.Normalize();
		Param.KnockbackForce = KnockbackDirectionOverride * KnockbackForceMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			Param.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude;
		}
	}

	if (bOverrideDeathImpulse)
	{
		KnockbackDirectionOverride.Normalize();
		Param.KnockbackForce = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
			DeathImpulseRotation.Pitch = PitchOverride;
			Param.KnockbackForce = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
	}

    if (bIsRadialDamage)
    {
    	Param.bIsRadialDamage = bIsRadialDamage;
    	Param.RadialDamageOrigin = InRadialDamageOrigin;
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
