// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FAuraWidgetControllerParam& WidgetControllerParam)
{
	// 如果主 UI 的控制器存在则返回，不存在则创建
	if (OverlayWidgetController == nullptr)
	{
		// 控制器需要 FAuraWidgetControllerParam 类型的数据
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerClass(WidgetControllerParam);
		// 绑定回调函数
		OverlayWidgetController->BindCallBacksDependencies();
	}
	return OverlayWidgetController;
}

UAttributeWidgetController* AAuraHUD::GetAttributeWidgetController(
	const FAuraWidgetControllerParam& WidgetControllerParam)
{
	if (AttributeWidgetController == nullptr)
	{
		AttributeWidgetController = NewObject<UAttributeWidgetController>(this, AttributeWidgetControllerClass);
		AttributeWidgetController->SetWidgetControllerClass(WidgetControllerParam);
		AttributeWidgetController->BindCallBacksDependencies();
	}
	return AttributeWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(
	const FAuraWidgetControllerParam& WidgetControllerParam)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerClass(WidgetControllerParam);
		SpellMenuWidgetController->BindCallBacksDependencies();
	}
	return SpellMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));
	
	// 创建主 UI
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	// 获得主 UI 的控制器，不存在则创建
	const FAuraWidgetControllerParam WidgetControllerParam(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParam); 

	// 将主 UI 的控制器设置为 WidgetController
	OverlayWidget->SetWidgetController(WidgetController);

	// 向所有小部件发出通知，初始化值
	WidgetController->BroadCastInitialValues();
 
	// 主 UI 添加到视口
	Widget->AddToViewport();
}
