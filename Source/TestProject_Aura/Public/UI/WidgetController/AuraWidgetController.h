// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FAuraWidgetControllerParam
{
	GENERATED_BODY();
	FAuraWidgetControllerParam(){}
	FAuraWidgetControllerParam(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS):PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS){}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
/**
 * WidgetController 控制所有的 Widget
 */
UCLASS()
class TESTPROJECT_AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	// 在 UI 创建时将对应的 UI 的 PC,PS,ASC,AS 数据设置给控制器，这样控制器才能知道并使用这些数据
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerClass(const FAuraWidgetControllerParam& WidgetControllerClassParam);

	virtual void BroadCastInitialValues();

	virtual void BindCallBacksDependencies();
protected:
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
	
};