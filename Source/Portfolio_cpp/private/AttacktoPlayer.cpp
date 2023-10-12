// Fill out your copyright notice in the Description page of Project Settings.


#include "AttacktoPlayer.h"
#include "Enemy.h"
#include "BossMonster.h"
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

	//SelfAcotr �����ͼ�
	UObject *tempObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"));
	
	//�θ�Ŭ������ �̸���
	FString ClassName = tempObject->GetClass()->GetSuperClass()->GetName();

	UE_LOG(LogTemp, Warning, TEXT("AttackToPlayer : %s"), *ClassName);

	//�����̳Ŀ� ���� �ٸ��� �� �����ؼ� 
	if (ClassName == FString("Enemy"))
	{
		ControlledEnemy = Cast<AEnemy>(tempObject);
		UE_LOG(LogTemp, Warning, TEXT("Enemy's Attack to Player Node is Activate"));
	}
	else if (ClassName == FString("BossMonster"))
	{
		ControlledEnemy = Cast<ABossMonster>(tempObject);
		UE_LOG(LogTemp, Warning, TEXT("BossMonster's Attack to Player Node is Activate"));
	}
	
	//�� ��ȯ ������ ��� Faliled 
	if (ControlledEnemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackToPlayer Node Failed, Cast Failed "));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	//��Ʈ�� �ǰ��ִ� ��ü�� Attack�Լ��� ��, Enemy�� ��ӹ޴� �Լ��� �������� �Ҹ� �ϵ��ڵ����� ��  
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