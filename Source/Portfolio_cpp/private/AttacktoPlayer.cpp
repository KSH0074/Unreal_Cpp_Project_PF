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

//블랙보드의 SelfActor의 값 == 컨트롤 되어지는 EnemyActor 를 ControlledEnemy에 할당 후 Attack 함수 실행
EBTNodeResult::Type UAttacktoPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//SelfAcotr 가져와서
	UObject *tempObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"));
	
	//부모클래스의 이름이
	FString ClassName = tempObject->GetClass()->GetSuperClass()->GetName();

	UE_LOG(LogTemp, Warning, TEXT("AttackToPlayer : %s"), *ClassName);

	//무엇이냐에 따라 다르게 값 대입해서 
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
	
	//형 변환 실패할 경우 Faliled 
	if (ControlledEnemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackToPlayer Node Failed, Cast Failed "));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	//컨트롤 되고있는 객체의 Attack함수를 콜, Enemy를 상속받는 함수가 많을수록 불리 하드코딩같이 됨  
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