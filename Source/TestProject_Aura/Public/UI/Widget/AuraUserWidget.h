// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 作为所有 Widget 的父类
 */
UCLASS()
class TESTPROJECT_AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 设置 WidgetController，在 MVC 模式中，WidgetController 作为控制所有 Widget 的控制器
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	// 保存 WidgetController
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};

