// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "testNode.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_CPP_API UtestNode : public UBTTaskNode
{
	UtestNode();
	GENERATED_BODY()
	
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	

};
