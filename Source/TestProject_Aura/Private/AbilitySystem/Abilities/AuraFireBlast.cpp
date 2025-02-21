// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 ScalableDamage = Damage.GetValueAtLevel(Level);;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);
	
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(""
							  "<Default>%s</> \nLevel:<Level>%d</> \nDamage:<Damage>%d</> \n"),
							  L"FireBlast Skill", Level, ScalableDamage);
	}
	else
	{
		return FString::Printf(TEXT(""
							  "<Default>%s</> \nLevel:<Level>%d</> \nDamage:<Damage>%d</> \n"),
							  L"FireBlast Skill", Level, ScalableDamage);
	}
	
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 ScaleDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);

	return FString::Printf(TEXT(""
							 "<Default>%s</> \nNextLevel Damage:<Damage>%d</> \n"),
							 L"FireBlast Skill", ScaleDamage);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;
	
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		FireBall->DamageEffectParam = MakeDamageEffectParamsFromClassDefault();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());

		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefault();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());

		FireBalls.Add(FireBall);

		FireBall->FinishSpawning(SpawnTransform);
	}

	return FireBalls;
}
