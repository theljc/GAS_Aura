// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers | Spawning")
	TSubclassOf<AActor> LootClass;
	
	UPROPERTY(EditAnywhere, Category = "LootTiers | Spawning")
	float ChanceToSpawn = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "LootTiers | Spawning")
	int32 MaxNumberToSpawn = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers | Spawning")
	bool bLootLevelOverride = true;
};

/**
 * 
 */
UCLASS()
class TESTPROJECT_AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootTiersItem();
	
	UPROPERTY(EditDefaultsOnly, Category = "LootTiers | Spawning")
	TArray<FLootItem> LootTiers;
	
};
