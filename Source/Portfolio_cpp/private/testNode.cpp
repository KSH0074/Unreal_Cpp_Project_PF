// Fill out your copyright notice in the Description page of Project Settings.


#include "testNode.h"
#include "..\public\testNode.h"

UtestNode::UtestNode()
{
	NodeName = TEXT("TestAttack");
}


	EBTNodeResult::Type UtestNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
	{
		FVector tagetLocation = Scene_Placed_PlayerPawn->GetActorLocation();

		FVector LoockDirection = tagetLocation - GetActorLocation();

		//Player�� �������� �ʴ� �� �þ߿����� ��������� �����ϵ��� �Ѵ�, SetActorRotation���� �ϸ� Controller�� ȸ������ �ʱ⶧���� GetController�� ����ؼ� ��Ʈ�ѷ��� ȸ�� 

		

		GetController()->SetControlRotation(LoockDirection.Rotation());

		currentTime = currentTime + GetWorld()->DeltaTimeSeconds;

		//AttackPlayer�� 
		if (currentTime > mAttackCoolTime)
		{
			//anim->bAttackPlay = true;
			int32 index = FMath::RandRange(0.0f, 1.9f);
			FString sectionName = FString::Printf(TEXT("Attack%d"), index);
			anim->PlayAttackAnim(FName(*sectionName));//BP���� ������ �Լ��� ����� 
			UE_LOG(LogTemp, Warning, TEXT("Enemy's Attack! %d"), index);
			currentTime = 0;
		}
		return EBTNodeResult::Succeeded;
	}
