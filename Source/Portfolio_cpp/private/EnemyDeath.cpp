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
	//�� �κе� AttackToPlayer�� ���� Tick�� ����ؼ� �и� ���
	ControlledEnemy = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
	ControlledEnemy->StopAnimMontage();
	ControlledEnemy->DeathState();
	return EBTNodeResult::Succeeded;
}
