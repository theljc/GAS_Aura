// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMacCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
	
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParameters, Vigor);
	// 限制 Vigor 不能低于 0
	Vigor = FMath::Max(Vigor, 0.F);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	// auto a = FString::Printf(TEXT("Vigor: %.f"), Vigor);
	// float aaa = 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
	// auto b = FString::Printf(TEXT("calc: %.f"), aaa);
	// GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, a);
	// GEngine->AddOnScreenDebugMessage(2, 5, FColor::Red, b);
	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
