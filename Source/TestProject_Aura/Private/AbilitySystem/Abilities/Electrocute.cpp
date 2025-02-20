// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 ScalableDamage = Damage.GetValueAtLevel(Level);;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);
	
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(""
							  "<Default>%s</> \nLevel:<Level>%d</> \nDamage:<Damage>%d</> \n"),
							  L"Electrocute Skill", Level, ScalableDamage);
	}
	else
	{
		return FString::Printf(TEXT(""
							  "<Default>%s</> \nLevel:<Level>%d</> \nDamage:<Damage>%d</> \n"),
							  L"Electrocute Skill", Level, ScalableDamage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 ScaleDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);

	return FString::Printf(TEXT(""
							 "<Default>%s</> \nNextLevel Damage:<Damage>%d</> \n"),
							 L"Electrocute Skill", ScaleDamage);
}
