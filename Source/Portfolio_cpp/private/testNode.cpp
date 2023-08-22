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

		//Player가 감지되지 않는 뒤 시야에서도 가까워지면 공격하도록 한다, SetActorRotation으로 하면 Controller는 회전하지 않기때문에 GetController를 사용해서 콘트롤러를 회전 

		

		GetController()->SetControlRotation(LoockDirection.Rotation());

		currentTime = currentTime + GetWorld()->DeltaTimeSeconds;

		//AttackPlayer가 
		if (currentTime > mAttackCoolTime)
		{
			//anim->bAttackPlay = true;
			int32 index = FMath::RandRange(0.0f, 1.9f);
			FString sectionName = FString::Printf(TEXT("Attack%d"), index);
			anim->PlayAttackAnim(FName(*sectionName));//BP에서 구현된 함수가 실행됨 
			UE_LOG(LogTemp, Warning, TEXT("Enemy's Attack! %d"), index);
			currentTime = 0;
		}
		return EBTNodeResult::Succeeded;
	}
