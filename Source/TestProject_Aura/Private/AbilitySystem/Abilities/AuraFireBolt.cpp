// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScalableDamage = Damage.GetValueAtLevel(Level);;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);
	
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(""
							  "<Default>%s</> \nLevel:<Level>%d</> \nDamage:<Damage>%d</> \n"),
							  L"FireBolt Skill", Level, ScalableDamage);
	}
	else
	{
		return FString::Printf(TEXT(""
							  "<Default>%s</> \nLevel:<Level>%d</> \nDamage:<Damage>%d</> \n"),
							  L"FireBolt Skill", Level, ScalableDamage);
	}

}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaleDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);

	return FString::Printf(TEXT(""
							 "<Default>%s</> \nNextLevel Damage:<Damage>%d</> \n"),
							 L"FireBolt Skill", ScaleDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsAuthority = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsAuthority) return;
	
	// 获得武器插槽的 Location
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator SocketRotation = (ProjectileLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		SocketRotation.Pitch = PitchOverride;
	}

	const FVector Forward = SocketRotation.Vector();

	const int32 EffectiveNumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());

	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Projectile->DamageEffectParam = MakeDamageEffectParamsFromClassDefault();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent =  NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::RandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;

		// 在所有属性设置完后生成
		Projectile->FinishSpawning(SpawnTransform);
	}

	
	
	// const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
	// const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);
	//
	//
	// // MaxNumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	//
	// if (NumProjectiles > 1)
	// {
	// 	const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
	// 	for (int32 i = 0; i < NumProjectiles; i++)
	// 	{
	// 		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
	// 		UKismetSystemLibrary::DrawDebugArrow(
	// 			GetAvatarActorFromActorInfo(),
	// 			SocketLocation, SocketLocation + Forward * 100.f,
	// 			1, FLinearColor::Yellow);
	//
	// 	}
	// }
	// else
	// {
	// 	
	// }
	//
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.f, 1, FLinearColor::Yellow);
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 1, FLinearColor::Yellow);
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 1, FLinearColor::Yellow);

}
