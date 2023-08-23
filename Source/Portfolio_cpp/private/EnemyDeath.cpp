// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDeath.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
UEnemyDeath::UEnemyDeath()
{
	NodeName = FString("Enemy Death and Delete");
}
EBTNodeResult::Type UEnemyDeath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ControlledEnemy = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
	ControlledEnemy->DeathState();
	return EBTNodeResult::Succeeded;
}
