// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMacCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParameters, Intelligence);
	// 限制 Vigor 不能低于 0
	Intelligence = FMath::Max(Intelligence, 0.F);

	int32 PlayerLevel = 1;

	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}
	
	// ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	// const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	// auto a = FString::Printf(TEXT("Intell: %.f"), Intelligence);
	// float aaa = 50.f + 2.5f * Intelligence + 15.f * PlayerLevel;
	// auto b = FString::Printf(TEXT("calc: %.f"), aaa);
	// GEngine->AddOnScreenDebugMessage(3, 5, FColor::Red, a);
	// GEngine->AddOnScreenDebugMessage(4, 5, FColor::Red, b);
	
	return 50.f + 2.5f * Intelligence + 15.f * PlayerLevel;
	
}
