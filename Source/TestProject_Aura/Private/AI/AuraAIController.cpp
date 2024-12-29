// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AuraAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AAuraAIController::AAuraAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	BehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
}
