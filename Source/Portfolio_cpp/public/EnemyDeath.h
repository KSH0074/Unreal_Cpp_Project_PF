// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyDeath.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_CPP_API UEnemyDeath : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UEnemyDeath();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	class AEnemy* ControlledEnemy{};
};
