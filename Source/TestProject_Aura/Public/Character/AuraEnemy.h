// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	public:
		AAuraEnemy();
		virtual void HighLightActor() override;
		virtual void UnHighLightActor() override;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Test")
		bool HighLight;
};