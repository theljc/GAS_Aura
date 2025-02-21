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
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"InputTag.LMB", FString("输入Tag，鼠标左键"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"InputTag.RMB", FString("输入Tag，鼠标右键"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"InputTag.1", FString("输入Tag，数字键1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"InputTag.2", FString("输入Tag，数字键2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"InputTag.3", FString("输入Tag，数字键3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"InputTag.4", FString("输入Tag，数字键4"));
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"InputTag.Passive.1", FString("输入Tag，被动技能数字键1"));
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			"InputTag.Passive.2", FString("输入Tag，被动技能数字键2"));
	
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

	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Debuff.Burn",
	FString("负面效果，燃烧伤害"));

	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Debuff.Stun",
	FString("负面效果，眩晕"));

	GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Debuff.Arcane",
	FString("负面效果，奥术伤害"));

	GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Debuff.Physical",
	FString("负面效果，物理伤害"));

	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Debuff.Chance",
	FString("负面效果几率"));

	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Debuff.Damage",
	FString("负面效果伤害"));

	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Debuff.Frequency",
	FString("负面效果周期"));

	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Debuff.Duration",
	FString("负面效果持续时间"));

	GameplayTags.DamageTypesToDebuffs.Add(
		GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Add(
		GameplayTags.Damage_Lighting, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(
		GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(
		GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);
	
	
	
	/* Meta Attribute */
	GameplayTags.Attribute_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Attribute.Meta.IncomingXP",
	FString("经验值"));

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

	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Summon",
	FString("召唤技能"));

	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.HitReact",
	FString("受击技能"));

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Status.Locked",
	FString("技能锁定状态"));

	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Status.Eligible",
	FString("技能可解锁状态"));

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Status.Unlocked",
	FString("技能解锁状态"));

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Status.Equipped",
	FString("技能装备状态"));

	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Type.None",
	FString("技能类型：无"));

	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Type.Offensive",
	FString("技能类型：攻击型"));

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Type.Passive",
	FString("技能类型：被动技能"));


	
	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Fire.FireBolt",
	FString("火球技能"));

	GameplayTags.Abilities_Lighting_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Lighting.Electrocute",
	FString("闪电技能"));

	GameplayTags.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Arcane.ArcaneShards",
	FString("奥术技能"));

	GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Abilities.Fire.FireBlast",
	FString("炎爆技能"));

	/*
	 * Passive Spells
	 */
	
	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.LifeSiphon"),
			FString("Life Siphon")
			);
	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.ManaSiphon"),
			FString("Mana Siphon")
			);
	GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.HaloOfProtection"),
			FString("光环保护")
			);
	
	// 冷却时间
	GameplayTags.CoolDown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"CoolDown.Fire.FireBolt",
	FString("火球技能冷却时间"));

	
	// Combat Socket
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"CombatSocket.Weapon",
	FString("用武器攻击的动画插槽"));

	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"CombatSocket.LeftHand",
	FString("用左手攻击的动画插槽"));
	
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"CombatSocket.RightHand",
	FString("用右手攻击的动画插槽"));

	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"CombatSocket.Tail",
	FString("用尾巴攻击的动画插槽"));

	// Montage Tags
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Montage.Attack.1",
	FString("攻击动画 1"));

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Montage.Attack.2",
	FString("攻击动画 2"));

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Montage.Attack.3",
	FString("攻击动画 3"));

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	"Montage.Attack.4",
	FString("攻击动画 4"));

	/*
	 * Player Tags
	 */

	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.CursorTrace"),
		FString("打断鼠标追踪")
		);

	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("打断 InputHeld")
		);

	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
		FString("打断 InputPressed")
		);

	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("打断 InputReleased")
		);

	
	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.FireBlast"),
		FString("炎爆技能的 GameplayCue")
		);
		
}
