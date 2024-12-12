// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerClass(const FAuraWidgetControllerParam& WidgetControllerClassParam)
{
	PlayerController = WidgetControllerClassParam.PlayerController;
	PlayerState = WidgetControllerClassParam.PlayerState;
	AbilitySystemComponent = WidgetControllerClassParam.AbilitySystemComponent;
	AttributeSet = WidgetControllerClassParam.AttributeSet;
	
}

void UAuraWidgetController::BroadCastInitialValues()
{
	
}

void UAuraWidgetController::BindCallBacksDependencies()
{
	
}
