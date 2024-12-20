// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{

}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorResult;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorResult);
	if (!CursorResult.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorResult.GetActor());

	/**
	 * 检测有如下 4 种情况
	 * 1. 上一个和这一个都为空：什么都不做
	 * 2. 上一个为空这一个不为空：高亮这一个
	 * 3. 上一个不为空这一个为空：取消高亮上一个
	 * 4. 上一个不为空这一个不为空：判断上一个是否等于这一个，相等则什么都不做，不相等则取消高亮上一个并高亮这一个
	 */
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// 高亮这一个
			ThisActor->HighLightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			// 取消高亮上一个
			LastActor->UnHighLightActor();

		}
		else
		{
			if (LastActor != ThisActor)
			{
				// 取消高亮上一个并高亮这一个
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}	
		}
	}
	
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AbilityInputTagPressed");
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AbilityInputTagReleased");
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GetASC()->AbilityInputTagHeld(InputTag);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "AbilityInputTagHeld");
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bReplicates = true;
	check(AuraContext);

	// 获得输入子系统
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());

	// 多人游戏时服务器上会有所有的 PlayerController，所以不能用 check 检查，需要用 if 判断
	if (InputSubsystem)
	{
		// 添加输入映射
		InputSubsystem->AddMappingContext(AuraContext, 0);
	}


	// 显示鼠标
	bShowMouseCursor = true;
	// 光标类型
	DefaultMouseCursor = EMouseCursor::Type::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 当鼠标光标被捕获到视口中时不隐藏
	InputModeData.SetHideCursorDuringCapture(false);
	// 设置初始输入模式
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	// 绑定 Move 行为
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	// 获得控制器 Yaw 轴方向
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	// 获得向前和向右方向的向量
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 获得控制器所控制的 Pawn
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 添加移动输入
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
	
}
