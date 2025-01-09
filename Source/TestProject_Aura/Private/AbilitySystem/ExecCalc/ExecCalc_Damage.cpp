// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	// 定义要捕获的属性
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightingResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightingResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);

		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

		TagsToCaptureDef.Add(Tags.Attribute_Secondary_Armor, ArmorDef);
		TagsToCaptureDef.Add(Tags.Attribute_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDef.Add(Tags.Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDef.Add(Tags.Attribute_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		TagsToCaptureDef.Add(Tags.Attribute_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDef.Add(Tags.Attribute_Resistance_Lighting, LightingResistanceDef);
		TagsToCaptureDef.Add(Tags.Attribute_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDef.Add(Tags.Attribute_Resistance_Physical, PhysicalResistanceDef);
		
	}
};

// 静态函数，用来获得结构体对象
static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DamageStatics;
	return DamageStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// 将要捕获的属性添加到数组中
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightingResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;

	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}

	int32 TargetPlayerLevel = 1;

	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	ICombatInterface* SourceCombatInterFace = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterFace = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParam;
	EvaluateParam.SourceTags = SourceTag;
	EvaluateParam.TargetTags = TargetTag;

	float Damage = 0.f;

	// 遍历伤害类型和伤害抗性的 Map
	for (auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistance)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;

		// 获得要捕获的属性的对象
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDef[ResistanceTag];

		// GetSetByCallerMagnitude 如果没有找到此伤害类型，会返回 0
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);	
		
		float Resistance = 0.f;
		// 捕获属性的值保存到 Resistance 变量中
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParam, Resistance);
		// 每点抗性抵挡 1% 伤害，所以不能高于 100
		Resistance = FMath::Clamp(Resistance, 0, 100);

		// 计算抗性
		DamageTypeValue *= ( 100 - Resistance) / 100.0f;
		
		// 结算抗性后的伤害
		Damage += DamageTypeValue;
	}
	

	float TargetBlockChance = 0.f;
	// 捕获属性
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParam, TargetBlockChance);
	// 限制不小于 0
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

	// 1 ~ 100 之间的随机数用于判断是否能够阻挡
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

	// 结算格挡后的伤害
	Damage = bBlocked ? Damage / 2.f : Damage;
	
	float TargetArmor = 0.f;
	// 捕获属性
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParam, TargetArmor);
	// 限制不小于 0
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);
	
	float SourceArmorPenetration = 0.f;
	// 捕获属性
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParam, SourceArmorPenetration);
	// 限制不小于 0
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);

	// 计算有效护甲
	const float EffectiveArmor = TargetArmor *= ( 100 - SourceArmorPenetration * ArmorPenetrationCoefficient ) / 100.f;

	
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);

	// 结算护甲后的伤害
	Damage *= ( 100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;


	float SourceCriticalHitChance = 0.f;
	// 捕获属性
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParam, SourceCriticalHitChance);
	// 限制不小于 0
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

	float SourceCriticalHitDamage = 0.f;
	// 捕获属性
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParam, SourceCriticalHitDamage);
	// 限制不小于 0
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);

	float TargetCriticalHitResistance = 0.f;
	// 捕获属性
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParam, TargetCriticalHitResistance);
	// 限制不小于 0
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	// 我方暴击率减去敌方抗暴率乘以等级
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	// 结算暴击后的伤害
	Damage = bCriticalHit ? Damage * 2.f + SourceCriticalHitDamage: Damage;
	
	// 施加伤害并应用修改
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetInComingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}
