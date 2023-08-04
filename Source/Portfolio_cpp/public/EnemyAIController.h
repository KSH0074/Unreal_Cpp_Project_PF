// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 이 C++파일과 이를 부모로하는 블루프린트 Enemy_Controller를 한 쪽으로 통일시킬 필요가 있음
 * 이는 리팩토링할때 반영바람 
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
	//데미지 입을 시 BlackBoard 값을 셋 하는 함수 추가 
	UFUNCTION(BlueprintCallable)
	void BlackboardIsDamagedSet(bool setIsDamaged);

	//FName 매개변수 대신 Enum을 통해서 사용할 수는 없나? 실험해보도록 한다.
	UFUNCTION(BlueprintCallable)
	void ChangeBlaockBoardState(FName S_stateName,bool setState);
};
