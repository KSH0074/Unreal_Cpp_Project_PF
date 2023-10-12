// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "test_BossController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_CPP_API Atest_BossController : public AEnemyAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
public:
	Atest_BossController();

	class ABossMonster* me;

};
