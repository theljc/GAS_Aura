// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAuraAbilitySystemComponent;
class UAbilityInfo;
struct FAuraAbilityInfo;

USTRUCT(BlueprintType)
struct FUIWidgetRow:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag GameplayTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UAuraUserWidget> MessageWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
	
};

class UAuraUserWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TESTPROJECT_AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	// 广播委托
	virtual void BroadCastInitialValues() override;
	// 回调函数
	virtual void BindCallBacksDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FMessageWidgetRowSignature MessageWidgetRow;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnAttributeChangedSignature LevelUpInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChanged;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	// void HealthChanged(const FOnAttributeChangeData& Data) const;
	// void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	// void ManaChanged(const FOnAttributeChangeData& Data) const;
	// void MaxManaChanged(const FOnAttributeChangeData& Data) const;

	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC);

	// 经验值改变时计算百分比
	void OnXPChanged(int32 NewXP);
	
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, FGameplayTag Tag);
	
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, FGameplayTag Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
