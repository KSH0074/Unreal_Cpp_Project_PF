// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	me = Cast<AEnemy>(InPawn);
}

AEnemyAIController::AEnemyAIController()
{
	
} 

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	
	if (me)
	{
		UE_LOG(LogTemp, Warning, TEXT("me is Exist"));

	}
	
	blackboardComp = GetBlackboardComponent();
	UE_LOG(LogTemp, Warning, TEXT("TestCode is BlackBoard Exist %s"), *blackboardComp->GetName());
	/*behaviorComp = Cast<UBehaviorTreeComponent>(BrainComponent);

	if (behaviorComp)
	{
		blackboardComp = behaviorComp->GetBlackboardComponent();
		if (blackboardComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("TestCode is BlackBoard Exist"));
		}
	}*/

}
//blueprint에서도 사용가능하도록 함 단, 매개변수로 bool 형을 받아 blackboardComp->SetValueAsBool(FName::FName("isDamaged"), 여기); 여기에다 적용하도록 만듦 
//ChangeBlackBoardState로 대체를 권장 
void AEnemyAIController::BlackboardIsDamagedSet(bool setIsDamaged)
{


	blackboardComp->SetValueAsBool(FName::FName("isDamaged"), setIsDamaged);
	
}


void AEnemyAIController::ChangeBlackBoardState(EEnemyState EState, bool setStateValue)
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

bool AEnemyAIController::getBlackBoardState(FName state)
{
	/*FName state{};
	UEnum::GetValueAsName(Estate, state);
	FString postFix = "Is";
	postFix = postFix + state.ToString();*/

	return blackboardComp->GetValueAsBool(state);
}


