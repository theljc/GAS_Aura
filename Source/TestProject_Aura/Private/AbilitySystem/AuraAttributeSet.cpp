// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"
#include "TestProject_Aura/AuraLogChannels.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	// InitHealth(20.f);
	// InitMaxHealth(100.f);
	// InitMana(40.f);
	// InitMaxMana(100.f);

	// 获得 GameplayTag
	const FAuraGameplayTags& GameplayTag = FAuraGameplayTags::Get();

	// 创建委托，绑定静态函数
	// FAttributeSignature StrengthDelegate;
	// StrengthDelegate.BindStatic(GetStrengthAttribute);
	
	// 添加 GameplayTag 和 函数指针 的映射

	// Primary Attribute
	TagsToAttribute.Add(GameplayTag.Attribute_Primary_Strength, GetStrengthAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Primary_Resilience, GetResilienceAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Primary_Vigor, GetVigorAttribute);

	// Secondary Attribute
	TagsToAttribute.Add(GameplayTag.Attribute_Secondary_Armor, GetArmorAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Secondary_MaxMana, GetMaxManaAttribute);

	// Resistance Attribute
	TagsToAttribute.Add(GameplayTag.Attribute_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Resistance_Lighting, GetLightingResistanceAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttribute.Add(GameplayTag.Attribute_Resistance_Physical, GetPhysicalResistanceAttribute);
	
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightingResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	

	// COND_None 表示无条件的复制，REPNOTIFY_Always 表示总是复制，默认是 REPNOTIFY_OnChanged，表示只在变量的值和原来不同时复制
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 限制生命和魔法值
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// 应用 GE 后 Data 中包含了很多信息
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// 获得触发 GE 的 ASC 中的信息
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		// 获得 AvatarActor
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		// 获得 PlayerController
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			// 判断是否能将 Actor 转换为 Pawn，如果可以则获得这个 Pawn 的 Controller
			if (const APawn* SourcePawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = SourcePawn->GetController();
			}
		}
		// 如果 Controller 存在，则将 Controller 控制的 Pawn 转换为 Character
		if (Props.SourceController != nullptr)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
	
	
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	// 将自定义结构体传入，获得 Data 里的数据
	SetEffectProperties(Data, EffectProperties);

	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		const float LocalInComingDamage = GetInComingDamage();
		SetInComingDamage(0.f);
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s,new attr: %f"), *EffectProperties.TargetAvatarActor->GetName(), LocalInComingDamage));
		if (LocalInComingDamage > 0)
		{
			const float NewHealth = GetHealth() - LocalInComingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0, GetMaxHealth()));

			const bool bFatal = NewHealth <= 0;
			if (bFatal)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
				SendXPEvent(EffectProperties);
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effect_HitReact);
				EffectProperties.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			
			const bool bBlocked = UAuraAbilitySystemLibrary::IsBlockedHit(EffectProperties.EffectContextHandle);
			const bool bCritical = UAuraAbilitySystemLibrary::IsCriticalHit(EffectProperties.EffectContextHandle);
			
			ShowFloatingText(EffectProperties, LocalInComingDamage, bBlocked, bCritical);
			
			
			// if (EffectProperties.SourceCharacter != EffectProperties.TargetCharacter)
			// {
			// 	AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(EffectProperties.SourceCharacter, 0));
			// 	if (AuraPC)
			// 	{
			// 		AuraPC->ShowDamageNumber(LocalInComingDamage, EffectProperties.TargetCharacter);
			// 	}
			// }
		}
	}
	if (Data.EvaluatedData.Attribute == GetInComingXPAttribute())
	{
		const float LocalInComingXP = GetInComingXP();
		SetInComingXP(0.f);
		if (EffectProperties.SourceCharacter->Implements<UPlayerInterface>() && EffectProperties.SourceCharacter->Implements<UCombatInterface>())
		{
			const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(EffectProperties.SourceCharacter);
			const int32 CurrentXP = IPlayerInterface::Execute_GetXP(EffectProperties.SourceCharacter);

			const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(EffectProperties.SourceCharacter, CurrentXP + LocalInComingXP);

			// 计算升了多少级
			const int32 NumsLevelUps = NewLevel - CurrentLevel;
			if (NumsLevelUps > 0)
			{
				/* 升级时的操作 */

				// 获得技能点
				const int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(EffectProperties.SourceCharacter, CurrentLevel);
				const int32 SpellPointsAward = IPlayerInterface::Execute_GetSpellPointsReward(EffectProperties.SourceCharacter, CurrentLevel);

				// 添加等级和技能点
				IPlayerInterface::Execute_AddToPlayerLevel(EffectProperties.SourceCharacter, NumsLevelUps);
				IPlayerInterface::Execute_AddToAttributePoints(EffectProperties.SourceCharacter, AttributePointsReward);
				IPlayerInterface::Execute_AddToSpellPoints(EffectProperties.SourceCharacter, SpellPointsAward);

				// 升级时生命值和魔法值回满
				SetHealth(GetMaxHealth());
				SetMana(GetMaxMana());
				
				IPlayerInterface::Execute_LevelUp(EffectProperties.SourceCharacter);
			}
			
			IPlayerInterface::Execute_AddToXP(EffectProperties.SourceCharacter, LocalInComingXP);
		}
	}
	
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& EffectProperties, float Damage, bool bBlockedHit, bool bCriticalHit)
{
	if (EffectProperties.SourceCharacter != EffectProperties.TargetCharacter)
	{
		if (AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(EffectProperties.SourceCharacter->Controller))
		{
			AuraPC->ShowDamageNumber(Damage, EffectProperties.TargetCharacter, bCriticalHit, bBlockedHit);
		}
		
		if (AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(EffectProperties.TargetCharacter->Controller))
		{
			AuraPC->ShowDamageNumber(Damage, EffectProperties.TargetCharacter, bCriticalHit, bBlockedHit);
		}
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Properties)
{
	if (Properties.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Properties.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Properties.TargetCharacter);
		const int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(Properties.TargetCharacter, TargetClass, TargetLevel);

		const FAuraGameplayTags& Tag = FAuraGameplayTags::Get();
		FGameplayEventData EventData;
		EventData.EventTag = Tag.Attribute_Meta_IncomingXP;
		EventData.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Properties.SourceCharacter, Tag.Attribute_Meta_IncomingXP, EventData);
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// 让 GAS 系统知道此变量被复制到客户端了
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
	
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	// 让 GAS 系统知道此变量被复制到客户端了
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
	
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	// 让 GAS 系统知道此变量被复制到客户端了
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
	
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	// 让 GAS 系统知道此变量被复制到客户端了
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet::OnRep_LightingResistance(const FGameplayAttributeData& OldLightingResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightingResistance, OldLightingResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}
