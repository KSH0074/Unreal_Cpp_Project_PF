// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
AEnemyAIController::AEnemyAIController()
{
	
}
//blueprint������ ��밡���ϵ��� �� ��, �Ű������� bool ���� �޾� blackboardComp->SetValueAsBool(FName::FName("isDamaged"), ����); ���⿡�� �����ϵ��� ���� 

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
