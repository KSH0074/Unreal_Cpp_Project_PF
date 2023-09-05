// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * �� C++���ϰ� �̸� �θ���ϴ� �������Ʈ Enemy_Controller�� �� ������ ���Ͻ�ų �ʿ䰡 ����
 * �̴� �����丵�Ҷ� �ݿ��ٶ� 
 */
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

//Enemy�� ������ �����ϰ� ����� ���� ���� 
//	UPROPERTY()
//	class AEnemy* me;

	UPROPERTY()
	class UBehaviorTreeComponent* behaviorComp;
	UPROPERTY()
	class UBlackboardComponent* blackboardComp;
	//������ ���� �� BlackBoard ���� �� �ϴ� �Լ� �߰� 
	UFUNCTION(BlueprintCallable)
	void BlackboardIsDamagedSet(bool setIsDamaged);

	//FName �Ű����� ��� Enum�� ���ؼ� ����� ���� ����? �����غ����� �Ѵ�.
	UFUNCTION(BlueprintCallable)
	void ChangeBlackBoardState(EEnemyState EState,bool setState);
	
	UFUNCTION()
	bool getBlackBoardState(FName state);
};
