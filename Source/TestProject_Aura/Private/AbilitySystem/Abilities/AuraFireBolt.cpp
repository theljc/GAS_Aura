// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);
	
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(""
							  "<Default>%s</> Level:<Level>%d</> Damage:<Damage>%d</>"),
							  L"FireBolt Skill", Level, Damage);
	}
	else
	{
		return FString::Printf(TEXT(""
							  "<Default>%s</> Level:<Level>%d</> Damage:<Damage>%d</>"),
							  L"FireBolt Skill", Level, Damage);
	}

}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);

	return FString::Printf(TEXT(""
							 "<Default>%s</> NextLevel Damage:<Damage>%d</>"),
							 L"FireBolt Skill", Damage);
}
