// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
AEnemyAIController::AEnemyAIController()
{
	
}
//blueprint에서도 사용가능하도록 함 단, 매개변수로 bool 형을 받아 blackboardComp->SetValueAsBool(FName::FName("isDamaged"), 여기); 여기에다 적용하도록 만듦 

void AEnemyAIController::BlackboardIsDamagedSet(bool setIsDamaged)
{

	behaviorComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (behaviorComp)
	{
		blackboardComp = behaviorComp->GetBlackboardComponent();
		if (blackboardComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("TestCode is BlackBoard Exist"))
		}
	}
	blackboardComp->SetValueAsBool(FName::FName("isDamaged"), setIsDamaged);
	
}
