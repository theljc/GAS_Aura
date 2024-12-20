// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// 重写 ICombatInterface 接口的函数
	virtual int32 GetPlayerLevel() override;
	
protected:
	virtual void BeginPlay() override;
	
private:
	// 初始化 ActorInfo
	virtual void InitAbilityActorInfo() override;
	
};
