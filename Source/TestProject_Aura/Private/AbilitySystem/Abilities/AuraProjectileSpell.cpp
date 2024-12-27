// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "LevelInstance/LevelInstanceTypes.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileLocation)
{
	const bool bisAuthority = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bisAuthority) return;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		// 获得武器插槽的 Location
		FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator SocketRotation = (ProjectileLocation - SocketLocation).Rotation();
		SocketRotation.Pitch = 0.f;

		// 设置 Location
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		//TODO：设置 Rotation
		SpawnTransform.SetRotation(SocketRotation.Quaternion());

		//延迟生成 Actor
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();

		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors);
		FHitResult Hit;
		EffectContextHandle.AddHitResult(Hit);
		Hit.Location = ProjectileLocation;
		
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

		const float ScaleDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Damage: %f") , ScaleDamage));

		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaleDamage);
		Projectile->DamageEffectSpecHandle = SpecHandle;
		
		// 在所有属性设置完后生成
		Projectile->FinishSpawning(SpawnTransform);
	}
	
}
