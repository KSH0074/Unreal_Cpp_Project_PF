// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Patrol,
	Damaged,
	Trace,
	Attack,
	Die,
	Toofar,
};
UCLASS()
class PORTFOLIO_CPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn *InPawn) override;
public:
	AEnemyAIController();

//Enemy에 접근을 용이하게 만들기 위한 변수 
	UPROPERTY(BlueprintReadOnly)
	class AEnemy* me;

	UPROPERTY(BlueprintReadWrite)
	class UBlackboardComponent* blackboardComp;

	//데미지 입을 시 BlackBoard 값을 셋 하는 함수 추가 
	UFUNCTION(BlueprintCallable)
	void BlackboardIsDamagedSet(bool setIsDamaged);

	UFUNCTION(BlueprintCallable)
	void ChangeBlackBoardState(EEnemyState EState,bool setState);
	
	UFUNCTION()
	bool getBlackBoardState(FName state);


};
