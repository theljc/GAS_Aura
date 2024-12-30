// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// 主要属性
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Primary.Strength", FString("主要属性，提升物理攻击"));
	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Primary.Intelligence", FString("主要属性，提升魔法攻击"));
	GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Primary.Resilience", FString("主要属性，护甲和护甲穿透"));
	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Primary.Vigor", FString("主要属性，提升生命值"));
	
	// 次要属性
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Secondary.MaxHealth", FString("次要属性，最大生命值"));
	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Secondary.MaxMana", FString("次要属性，最大魔法值"));
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Secondary.Armor", FString("次要属性，护甲值"));
	GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Secondary.ArmorPenetration", FString("次要属性，护甲穿透值"));
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Secondary.BlockChance", FString("次要属性，减伤几率"));
	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Secondary.CriticalHitChance", FString("次要属性，暴击率"));
	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Secondary.CriticalHitDamage", FString("次要属性，暴击伤害"));
	GameplayTags.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Secondary.CriticalHitResistance", FString("次要属性，抗暴率"));
	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Secondary.HealthRegeneration", FString("次要属性，每秒回血量"));
	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag("Attribute.Secondary.ManaRegeneration", FString("次要属性，每秒回蓝量"));
	
	// 输入 Tag
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.LMB", FString("输入Tag，鼠标左键"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.RMB", FString("输入Tag，鼠标右键"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.1", FString("输入Tag，数字键1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.2", FString("输入Tag，数字键2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.3", FString("输入Tag，数字键3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.4", FString("输入Tag，数字键4"));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"Damage",
		FString("伤害"));

	// 伤害类型
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"Damage.Fire",
		FString("火焰伤害类型"));

	GameplayTags.Damage_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"Damage.Lighting",
		FString("闪电伤害类型"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"Damage.Arcane",
		FString("奥术伤害类型"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"Damage.Physical",
		FString("物理伤害类型"));
	
	// 伤害类型抗性
	GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Attribute.Resistance.Fire",
	FString("火焰伤害抗性"));
	GameplayTags.Attribute_Resistance_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Attribute.Resistance.Lighting",
	FString("闪电伤害抗性"));
	GameplayTags.Attribute_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Attribute.Resistance.Arcane",
	FString("奥术伤害抗性"));
	GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Attribute.Resistance.Physical",
	FString("物理伤害抗性"));
	

	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Fire, GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Lighting, GameplayTags.Attribute_Resistance_Lighting);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Arcane, GameplayTags.Attribute_Resistance_Arcane);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Physical, GameplayTags.Attribute_Resistance_Physical);
	
	
	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Effect.HitReact",
	FString("受伤时的行为"));

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Attack",
	FString("攻击技能"));

	GameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Montage.Attack.Weapon",
	FString("用武器攻击的动画"));

	GameplayTags.Montage_Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Montage.Attack.LeftHand",
	FString("用左手攻击的动画"));
	
	GameplayTags.Montage_Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Montage.Attack.RightHand",
	FString("用右手攻击的动画"));
	
}
