// Fill out your copyright notice in the Description page of Project Settings.


#include "AttacktoPlayer.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
UAttacktoPlayer::UAttacktoPlayer()
{
	NodeName = FString("AttacktoPlayer");
}

//블랙보드의 SelfActor의 값 == 컨트롤 되어지는 EnemyActor 를 ControlledEnemy에 할당 후 Attack 함수 실행
EBTNodeResult::Type UAttacktoPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ControlledEnemy = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
	
	//형 변환 실패할 경우 Faliled 
	if (ControlledEnemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackToPlayer Node Failed, Cast Failed "));
		return EBTNodeResult::Failed;
	}
	ControlledEnemy->Attack();
	//Succeeded를 해야 이 부분이 계속 돌아감=> 사실상 TIck 과 같다  
	return EBTNodeResult::Succeeded;
	
}
