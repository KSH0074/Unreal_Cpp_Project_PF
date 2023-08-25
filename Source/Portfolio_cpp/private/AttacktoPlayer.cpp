// Fill out your copyright notice in the Description page of Project Settings.


#include "AttacktoPlayer.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
UAttacktoPlayer::UAttacktoPlayer()
{
	NodeName = FString("AttacktoPlayer");
}

EBTNodeResult::Type UAttacktoPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	
	ControlledEnemy = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
	ControlledEnemy->AttacktoPlayer();
	
	return EBTNodeResult::Succeeded;
}
