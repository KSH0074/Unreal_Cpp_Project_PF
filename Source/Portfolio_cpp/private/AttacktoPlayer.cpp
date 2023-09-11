// Fill out your copyright notice in the Description page of Project Settings.


#include "AttacktoPlayer.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
UAttacktoPlayer::UAttacktoPlayer()
{
	NodeName = FString("AttacktoPlayer");
	bNotifyTick = true;
}

//�������� SelfActor�� �� == ��Ʈ�� �Ǿ����� EnemyActor �� ControlledEnemy�� �Ҵ� �� Attack �Լ� ����
EBTNodeResult::Type UAttacktoPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ControlledEnemy = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
	
	//�� ��ȯ ������ ��� Faliled 
	if (ControlledEnemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackToPlayer Node Failed, Cast Failed "));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	if (!ControlledEnemy->isMontagePlaying)
	{
		ControlledEnemy->Attack();
	}
	

	return EBTNodeResult::InProgress;
	
}
void UAttacktoPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	//UE_LOG(LogTemp, Warning, TEXT("Attack Task tick"));

	if (!ControlledEnemy -> isMontagePlaying)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
}