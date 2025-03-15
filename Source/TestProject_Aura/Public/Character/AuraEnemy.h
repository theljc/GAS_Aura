// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class TESTPROJECT_AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	virtual void PossessedBy(AController* NewController) override;

	/* Highlight Interface */
	virtual void HighLightActor_Implementation() override;
	virtual void UnHighLightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;

	// 重写 ICombatInterface 接口的函数
	virtual int32 GetPlayerLevel_Implementation() override;
	
	void HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount);

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;

	virtual AActor* GetCombatTarget_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Test")
	bool HighLight;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	// 初始化敌人类的属性
	virtual void InitializeDefaultAttributes() const override;

	virtual void Die(const FVector& DeathImpluse) override;

	virtual void StunTagChanged(const FGameplayTag CallBackTag, int32 NewCount) override;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Default Class")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
	
};
