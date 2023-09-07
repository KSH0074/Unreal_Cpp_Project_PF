// Fill out your copyright notice in the Description page of Project Settings.


#include "AttacktoPlayer.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
UAttacktoPlayer::UAttacktoPlayer()
{
	NodeName = FString("AttacktoPlayer");
}

//�������� SelfActor�� �� == ��Ʈ�� �Ǿ����� EnemyActor �� ControlledEnemy�� �Ҵ� �� Attack �Լ� ����
EBTNodeResult::Type UAttacktoPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ControlledEnemy = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
	
	//�� ��ȯ ������ ��� Faliled 
	if (ControlledEnemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackToPlayer Node Failed, Cast Failed "));
		return EBTNodeResult::Failed;
	}
	ControlledEnemy->Attack();
	//Succeeded�� �ؾ� �� �κ��� ��� ���ư�=> ��ǻ� TIck �� ����  
	return EBTNodeResult::Succeeded;
	
}
