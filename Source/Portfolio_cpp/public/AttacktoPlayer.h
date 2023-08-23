// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttacktoPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_CPP_API UAttacktoPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UAttacktoPlayer();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	class AEnemy* ControlledEnemy{};
};
