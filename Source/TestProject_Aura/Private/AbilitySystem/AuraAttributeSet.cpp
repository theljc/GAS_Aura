// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

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

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
	
}

void UAuraAttributeSet::HandleInComingDamage(const FEffectProperties& Props)
{
	const float LocalInComingDamage = GetInComingDamage();
	SetInComingDamage(0.f);
	if (LocalInComingDamage > 0)
	{
		const float NewHealth = GetHealth() - LocalInComingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0;
		if (bFatal)
		{
			
			
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpluse(Props.EffectContextHandle));
			}
			SendXPEvent(Props);
		}
		else
		{
			// 不处于麻痹状态时才会受击
			if (Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetCharacter))
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effect_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const FVector& KnockbackForce = UAuraAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
				
			}
		}

			
		const bool bBlocked = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCritical = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
			
		ShowFloatingText(Props, LocalInComingDamage, bBlocked, bCritical);
			
		if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			Debuff(Props);
		}
		
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

void UAuraAttributeSet::HandleInComingXP(const FEffectProperties& Props)
{
	const float LocalInComingXP = GetInComingXP();
	SetInComingXP(0.f);
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalInComingXP);

		// 计算升了多少级
		const int32 NumsLevelUps = NewLevel - CurrentLevel;
		if (NumsLevelUps > 0)
		{
			/* 升级时的操作 */

			// 添加等级和技能点
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumsLevelUps);

			int32 AttributePointsReward = 0;
			int32 SpellPointsAward = 0;
			for (int32 i = 0; i < NumsLevelUps; i++)
			{
				// 获得技能点
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel + i);
				SpellPointsAward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel + i);
			}
			
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsAward);

			// 升级时生命值和魔法值回满，此函数会在 PostAttributeChange 之前调用
			bTopOffHealth = true;
			bTopOffMana = true;
				
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
			
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalInComingXP);
	}
}

void UAuraAttributeSet::Debuff(const FEffectProperties& Props)
{
	FGameplayEffectContextHandle ContextHandle = Props.SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
	FString DebuffName = FString::Printf(TEXT("动态减益效果：%s"), *DamageType.ToString());
	// 动态创建 GE
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	// 设置 GE 的各个参数
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

	
	// Granted Tag
	// Effect->InheritableOwnedTagsContainer.AddTag(AuraGameplayTags.DamageTypesToDebuffs[DamageType]);
	const FGameplayTag DebuffTag = AuraGameplayTags.DamageTypesToDebuffs[DamageType];
	UTargetTagsGameplayEffectComponent& GameplayEffectComponent = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer InheritedTagContainer = FInheritedTagContainer();
	InheritedTagContainer.Added.AddTag(DebuffTag);
	InheritedTagContainer.CombinedTags.AddTag(DebuffTag);
	GameplayEffectComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);

	if (DebuffTag.MatchesTagExact(AuraGameplayTags.Debuff_Stun))
	{
		UTargetTagsGameplayEffectComponent& GameplayEffectComponent_Stun = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
		FInheritedTagContainer InheritedTagContainer_Stun = FInheritedTagContainer();
		InheritedTagContainer_Stun.Added.AddTag(AuraGameplayTags.Player_Block_CursorTrace);
		InheritedTagContainer_Stun.Added.AddTag(AuraGameplayTags.Player_Block_InputHeld);
		InheritedTagContainer_Stun.Added.AddTag(AuraGameplayTags.Player_Block_InputPressed);
		InheritedTagContainer_Stun.Added.AddTag(AuraGameplayTags.Player_Block_InputReleased);
		InheritedTagContainer_Stun.CombinedTags.AddTag(AuraGameplayTags.Player_Block_CursorTrace);
		InheritedTagContainer_Stun.CombinedTags.AddTag(AuraGameplayTags.Player_Block_InputHeld);
		InheritedTagContainer_Stun.CombinedTags.AddTag(AuraGameplayTags.Player_Block_InputPressed);
		InheritedTagContainer_Stun.CombinedTags.AddTag(AuraGameplayTags.Player_Block_InputReleased);
		GameplayEffectComponent_Stun.SetAndApplyTargetTagChanges(InheritedTagContainer_Stun);
	}
	
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 idx = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[idx];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UAuraAttributeSet::GetInComingDamageAttribute();

	// 用 GE 创建 Spec
	FGameplayEffectSpec* EffectSpec = new FGameplayEffectSpec(Effect, ContextHandle, 1.f);
	if (EffectSpec)
	{
		FAuraGameplayEffectContext* AuraGameplayContext = static_cast<FAuraGameplayEffectContext*>(EffectSpec->GetContext().Get());
		// 创建 TSharedPtr 类型，用 MakeShareable 创建新的
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		AuraGameplayContext->SetDamageType(DebuffDamageType);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec);
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

	// 判断目标是否死亡
	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>())
	{
		if (ICombatInterface::Execute_IsDead(EffectProperties.TargetCharacter))
		{
			return;
		}
	}
	
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
		HandleInComingDamage(EffectProperties);
	}
	if (Data.EvaluatedData.Attribute == GetInComingXPAttribute())
	{
		HandleInComingXP(EffectProperties);
		
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
