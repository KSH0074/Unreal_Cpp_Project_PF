// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * �� C++���ϰ� �̸� �θ���ϴ� �������Ʈ Enemy_Controller�� �� ������ ���Ͻ�ų �ʿ䰡 ����
 * �̴� �����丵�Ҷ� �ݿ��ٶ� 
 */
UCLASS()
class PORTFOLIO_CPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	UPROPERTY()
	class UBehaviorTreeComponent* behaviorComp;
	UPROPERTY()
	class UBlackboardComponent* blackboardComp;
	//������ ���� �� BlackBoard ���� �� �ϴ� �Լ� �߰� 
	UFUNCTION(BlueprintCallable)
	void BlackboardIsDamagedSet(bool setIsDamaged);

	//FName �Ű����� ��� Enum�� ���ؼ� ����� ���� ����? �����غ����� �Ѵ�.
	UFUNCTION(BlueprintCallable)
	void ChangeBlaockBoardState(FName S_stateName,bool setState);
};
