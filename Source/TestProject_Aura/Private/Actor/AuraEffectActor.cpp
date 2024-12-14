// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	// SetRootComponent(Mesh);
	// Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	// Sphere->SetupAttachment(GetRootComponent());
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 用蓝图函数库里的函数获得 ASC 组件
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	// ASC 组件可以为 NULL，GameplayEffect 不能为 NULL
	if (TargetASC == nullptr) return;
	check(GameplayEffectClass);
	// 创建 EffectContextHandle
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	// 设置触发 GE 的源 Object 是自己
	EffectContextHandle.AddSourceObject(this);
	// 创建 GameplayEffectSpecHandle
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	// 通过 Handle 应用 GE，Handle 中的 Data 是 TSharedPtr<FGameplayEffectSpec>，Get 函数获得 FGameplayEffectSpec 指针再解引用
	const FActiveGameplayEffectHandle ActiveGameplayEffect = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	// 应用 GE 时判断这个 GE 是否是 Infinite 且设置为了 RemoveOnEndOverlap，则添加到 Map 中用于之后手动移除此 GE
	const bool bIsInfinite =  EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveGameplayEffect, TargetASC);
	}
}

// void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
// 	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	// 判断碰撞者是否拥有 IAbilitySystemInterface 接口
// 	if (IAbilitySystemInterface* AscInterface = Cast<IAbilitySystemInterface>(OtherActor))
// 	{
// 		// 拥有接口则获得对应的 ASC 组件中的 AttributeSet
// 		const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AscInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
// 		// TODO：AttributeSet 是 const 类型，所以转换为非 const 之后才能修改属性
// 		UAuraAttributeSet* MutableAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
// 		MutableAuraAttributeSet->SetHealth(MutableAuraAttributeSet->GetHealth() + 25.f);
// 		MutableAuraAttributeSet->SetMana(MutableAuraAttributeSet->GetMana() - 25.f);
//
// 		// 碰撞后销毁自己
// 		Destroy();
// 	}
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, "未插入接口");
// 	}
// 	
// }

// void AAuraEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	
// }

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
//
// 	// 为球体碰撞绑定委托
// 	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
// 	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnEndOverlap);
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	// 重叠时根据不同的持续时间应用 GE
	if (InstanceEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstanceGameplayEffect);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffect);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffect);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// 取消重叠时根据不同的持续时间应用 GE
	if (InstanceEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstanceGameplayEffect);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffect);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffect);
	}
	// 持续时间无限的 GE 在取消重叠时判断是否需要移除
	if (InfiniteRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;
		// 数组用来保存被移除的 GE，因为在 Map 遍历过程中不能直接移除 Map 中的键值对
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		// 遍历 Map，找到要移除的 GE 对应的 ASC 组件后移除
		for (auto HandlePair: ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				// 移除时移除一层效果
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		// 遍历完后移除 Map 中对应的键值对
		for (auto Handle: HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
		
	}
}

