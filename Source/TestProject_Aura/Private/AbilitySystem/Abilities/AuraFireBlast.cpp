// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScalableDamage = Damage.GetValueAtLevel(Level);;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);
	
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(""
							  "<Default>%s</> \nLevel:<Level>%d</> \nDamage:<Damage>%d</> \n"),
							  L"FireBlast Skill", Level, ScalableDamage);
	}
	else
	{
		return FString::Printf(TEXT(""
							  "<Default>%s</> \nLevel:<Level>%d</> \nDamage:<Damage>%d</> \n"),
							  L"FireBlast Skill", Level, ScalableDamage);
	}
	
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaleDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);

	return FString::Printf(TEXT(""
							 "<Default>%s</> \nNextLevel Damage:<Damage>%d</> \n"),
							 L"FireBlast Skill", ScaleDamage);
}
