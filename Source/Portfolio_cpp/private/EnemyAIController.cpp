// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
AEnemyAIController::AEnemyAIController()
{
	//������ �ܰ迡�� �Ʒ� �ڵ尡 �۵����� �ʴ´�. BeginPlay�� �ű�ų� �ٸ� ����� �����Ѵ�.	
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
//blueprint������ ��밡���ϵ��� �� ��, �Ű������� bool ���� �޾� blackboardComp->SetValueAsBool(FName::FName("isDamaged"), ����); ���⿡�� �����ϵ��� ���� 

void AEnemyAIController::BlackboardIsDamagedSet(bool setIsDamaged)
{


	blackboardComp->SetValueAsBool(FName::FName("isDamaged"), setIsDamaged);
	
}

void AEnemyAIController::ChangeBlaockBoardState(FName S_stateName, bool setStateValue)
{
	blackboardComp->SetValueAsBool(FName::FName(S_stateName), setStateValue);
}

