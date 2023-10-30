// Fill out your copyright notice in the Description page of Project Settings.


#include "test_BossController.h"
#include "BossMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
void Atest_BossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	me = Cast<ABossMonster>(InPawn);
}
Atest_BossController::Atest_BossController()
{
}
void Atest_BossController::ChangeBlackBoardState_Boss(EBossState EState, bool setStateValue)
{

	switch (EState)
	{
	case EBossState::Idle:
		GetBlackboardComponent()->SetValueAsBool("IsIdle", setStateValue);
		break;
	case EBossState::Patrol:
		GetBlackboardComponent()->SetValueAsBool("IsPatrol", setStateValue);
		break;
	case EBossState::Damaged:
		GetBlackboardComponent()->SetValueAsBool("IsDamaged", setStateValue);
		break;
	case EBossState::Trace:
		GetBlackboardComponent()->SetValueAsBool("IsTrace", setStateValue);
		break;
	case EBossState::Attack:
		GetBlackboardComponent()->SetValueAsBool("IsAttack", setStateValue);
		break;
	case EBossState::Die:
		GetBlackboardComponent()->SetValueAsBool("IsDead", setStateValue);
		break;
	case EBossState::Toofar:
		GetBlackboardComponent()->SetValueAsBool("IsToofar", setStateValue);
		break;
	case EBossState::Charge:
		GetBlackboardComponent()->SetValueAsBool("IsCharge", setStateValue);
		break;
	default:
		break;
	}

	
}
void Atest_BossController::BeginPlay()
{
	Super::BeginPlay();

}