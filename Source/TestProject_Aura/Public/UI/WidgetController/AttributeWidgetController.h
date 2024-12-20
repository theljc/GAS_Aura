// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AuraDataAsset.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeWidgetController.generated.h"

struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, NewAttributeInfo);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TESTPROJECT_AURA_API UAttributeWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BindCallBacksDependencies() override;
	virtual void BroadCastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category="Ability System|Attribute")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAuraDataAsset> AttributeInfo;

private:
	void BroadcastAttributeValues(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
	
};
