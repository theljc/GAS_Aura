// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SplineComponent.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;


/**
 * 
 */
UCLASS()
class TESTPROJECT_AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	virtual void PlayerTick(const float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(const float DamageAmount, ACharacter* TargetCharacter, bool bCriticalHit, bool bBlockedHit);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	// 按住 shift 时可以在指定位置释放技能
	void ShiftPressed() {bShiftKeyDown = true;}
	void ShiftReleased() {bShiftKeyDown = false;}
	bool bShiftKeyDown = false;
	
	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	// TObjectPtr<AActor> ThisActor;
	// TObjectPtr<AActor> LastActor;
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	
	FHitResult CursorHitResult;

	// 调用 AuraASC 组件的函数来处理按下，松开，按住按键时是否激活 GA
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UAuraAbilitySystemComponent* GetASC();

	// 自动寻路
	void AutoRun();
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	// 缓存的要到达的目标位置
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.0f;
	// 短按的时间间隔
	float ShortPressThreshold = 0.5f;
	// 是否自动移动
	bool bAutoRunning = false;
	// 鼠标左键如果点击到了敌人，则会激活 GA 并将该敌人作为目标，此时不会进行移动
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	// 样条线用于自动寻路
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	
};


