// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "TestProject_Aura/TestProject_Aura.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	// FName 类型不需要加 TEXT 宏
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MultiCastHandleDeath_Implementation();
}

void AAuraCharacterBase::MultiCastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);;
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);

	// 死亡后溶解
	Dissolve();

	bDead = true;
	
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
	
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
	
	if (MontageTag.MatchesTagExact(AuraGameplayTags.Montage_Attack_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	
	if (MontageTag.MatchesTagExact(AuraGameplayTags.Montage_Attack_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}

	if (MontageTag.MatchesTagExact(AuraGameplayTags.Montage_Attack_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}

	return FVector();
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> DefaultGameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultGameplayEffectClass);

	// 应用 GE
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultGameplayEffectClass, Level, EffectContextHandle);
	FActiveGameplayEffectHandle test = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), GetAbilitySystemComponent());
	if (!test.WasSuccessfullyApplied())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, DefaultGameplayEffectClass->GetName() + FString(TEXT(" 失败")));
		
	}
	// FString str = FString::Printf(TEXT("%s"), DefaultPrimaryGameplayEffectClass->GetName());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryGameplayEffectClass, 1.f);
	ApplyEffectToSelf(DefaultSecondaryGameplayEffectClass, 1.f);
	ApplyEffectToSelf(DefaultVitalGameplayEffectClass, 1.f);
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority())
	{
		return;
	}
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->AddCharacterAbilities(StartUpAbilities);
	
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
	
}
