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
	//생성자 단계에서 아래 코드가 작동되지 않는다. BeginPlay에 옮기거나 다른 방법을 강구한다.	
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
//blueprint에서도 사용가능하도록 함 단, 매개변수로 bool 형을 받아 blackboardComp->SetValueAsBool(FName::FName("isDamaged"), 여기); 여기에다 적용하도록 만듦 
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


