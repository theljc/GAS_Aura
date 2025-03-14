// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                     const FGameplayEffectSpec& Spec,
	                     FAggregatorEvaluateParameters EvaluateParam,
	                     const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDef) const;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
