// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
AEnemyAIController::AEnemyAIController()
{
	//생성자 단계에서 아래 코드가 작동되지 않는다. BeginPlay에 옮기거나 다른 방법을 강구한다.	
	behaviorComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	blackboardComp = behaviorComp->GetBlackboardComponent();
	if (behaviorComp)
	{
		blackboardComp = behaviorComp->GetBlackboardComponent();
		if (blackboardComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("TestCode is BlackBoard Exist"))
		}
	}
}
//blueprint에서도 사용가능하도록 함 단, 매개변수로 bool 형을 받아 blackboardComp->SetValueAsBool(FName::FName("isDamaged"), 여기); 여기에다 적용하도록 만듦 

void AEnemyAIController::BlackboardIsDamagedSet(bool setIsDamaged)
{


	blackboardComp->SetValueAsBool(FName::FName("isDamaged"), setIsDamaged);
	
}

void AEnemyAIController::ChangeBlaockBoardState(FName S_stateName, bool setStateValue)
{
	blackboardComp->SetValueAsBool(FName::FName(S_stateName), setStateValue);
}

