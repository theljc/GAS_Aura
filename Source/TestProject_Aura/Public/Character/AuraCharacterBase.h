// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;


UCLASS(Abstract)
class TESTPROJECT_AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet;}
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/* Combat Interface */
	virtual void Die(const FVector& DeathImpluse) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(const int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual FOnDeath& GetOnDeathDelegate() override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override;
	virtual bool IsBeingShocked_Implementation() const override;
	/* end Combat Interface */

	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MultiCastHandleDeath(const FVector& DeathImpluse);

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;

	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;

	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

	void SetCharacterClass(ECharacterClass InClass) { CharacterClass = InClass; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;
	
	virtual void StunTagChanged(const FGameplayTag CallBackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 600.f;

	UPROPERTY(Category = "Combat", EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(Category = "Combat", EditAnywhere)
	FName WeaponTipSocketName;

	UPROPERTY(Category = "Combat", EditAnywhere)
	FName LeftHandSocketName;

	UPROPERTY(Category = "Combat", EditAnywhere)
	FName RightHandSocketName;

	UPROPERTY(Category = "Combat", EditAnywhere)
	FName TailSocketName;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultPrimaryGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultSecondaryGameplayEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultVitalGameplayEffectClass;

	// 应用 GE
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> DefaultGameplayEffectClass, float Level) const;
	// 初始化所有的默认属性
	virtual void InitializeDefaultAttributes() const;
	// 只在服务器上执行，此函数会调用 AuraASC 的 AddCharacterAbilities 激活初始 GA
	void AddCharacterAbilities() const;

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* DeathSound;

	/* Minions */

	int32 MinionCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Default Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;
	
private:
	UPROPERTY(EditAnywhere, category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;

	UPROPERTY(EditAnywhere, category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartUpPassiveAbilities;

	UPROPERTY(EditAnywhere, category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;

};
