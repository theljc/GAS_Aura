// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraSummonAbility.h"


TArray<FVector> UAuraSummonAbility::GetSpawnLocation()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	TArray<FVector> SpawnLocations;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);

		FVector ChooseSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		// DrawDebugSphere(GetWorld(), ChooseSpawnLocation, 10, 8, FColor::Cyan, false, 4.f);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, ChooseSpawnLocation + FVector(0,0, 400), ChooseSpawnLocation - FVector(0,0, 400), ECC_Visibility);
		if (HitResult.bBlockingHit)
		{
			ChooseSpawnLocation = HitResult.ImpactPoint;
		}
		
		SpawnLocations.Add(ChooseSpawnLocation);
		
		// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location + Direction * MinSpawnDistance, Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Red, 4.f);
		// DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 10, 8, FColor::Yellow, false, 4.f);
		// DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 10, 8, FColor::Yellow, false, 4.f);
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 Section = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Section];
}
