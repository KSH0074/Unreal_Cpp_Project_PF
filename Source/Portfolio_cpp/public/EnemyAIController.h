// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_CPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	UPROPERTY()
	class UBlackboardComponent* BlackboardComp;
	//데미지 입을 시 BlackBoard 값을 셋 하는 함수 추가 
};
