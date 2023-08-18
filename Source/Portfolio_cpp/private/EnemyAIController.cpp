// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{

}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	//me = Cast<AEnemy>(GetPawn());
	//if (me)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("me is Exist"));

	//}
	//
	//������ �ܰ迡�� �Ʒ� �ڵ尡 �۵����� �ʴ´�. BeginPlay�� �ű�ų� �ٸ� ����� �����Ѵ�.	
	behaviorComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	blackboardComp = behaviorComp->GetBlackboardComponent();
	if (behaviorComp)
	{
		blackboardComp = behaviorComp->GetBlackboardComponent();
		if (blackboardComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("TestCode is BlackBoard Exist"));
		}
	}

}
//blueprint������ ��밡���ϵ��� �� ��, �Ű������� bool ���� �޾� blackboardComp->SetValueAsBool(FName::FName("isDamaged"), ����); ���⿡�� �����ϵ��� ���� 
void AEnemyAIController::BlackboardIsDamagedSet(bool setIsDamaged)
{


	blackboardComp->SetValueAsBool(FName::FName("isDamaged"), setIsDamaged);
	
}


void AEnemyAIController::ChangeBlaockBoardState(EEnemyState EState, bool setStateValue)
{
	switch (EState)
	{
	case EEnemyState::Idle:
		blackboardComp->SetValueAsBool("IsIdle", setStateValue);
		break;
	case EEnemyState::Patrol:
		blackboardComp->SetValueAsBool("IsPatrol", setStateValue);
		break;
	case EEnemyState::Damaged:
		blackboardComp->SetValueAsBool("IsDamaged", setStateValue);
		break;
	case EEnemyState::Trace:
		blackboardComp->SetValueAsBool("IsTrace", setStateValue);
		break;
	case EEnemyState::Attack:
		blackboardComp->SetValueAsBool("IsAttack", setStateValue);
		break;
	case EEnemyState::Die:
		blackboardComp->SetValueAsBool("IsDead", setStateValue);
		break;
	case EEnemyState::Toofar:
		blackboardComp->SetValueAsBool("IsToofar", setStateValue);
		break;
	default:
		break;
	}

}


