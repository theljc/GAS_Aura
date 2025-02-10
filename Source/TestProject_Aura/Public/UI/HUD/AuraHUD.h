// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class USpellMenuWidgetController;
class UAttributeWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UAuraUserWidget;
struct FAuraWidgetControllerParam;
/**
 * 
 */
UCLASS()
class TESTPROJECT_AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:


	// 获得主 UI 的控制器，控制器只有一个实例，保存了 PC,PS,ASC,AS 的数据
	UOverlayWidgetController* GetOverlayWidgetController(const FAuraWidgetControllerParam& WidgetControllerParam);
	// 获得 Attribute UI 的控制器
	UAttributeWidgetController* GetAttributeWidgetController(const FAuraWidgetControllerParam& WidgetControllerParam);
	
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FAuraWidgetControllerParam& WidgetControllerParam);

	// 初始化主 UI
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	
private:
	// 作为主 UI 显示在屏幕上
    UPROPERTY()
    TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass; 

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeWidgetController> AttributeWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeWidgetController> AttributeWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
	
};
