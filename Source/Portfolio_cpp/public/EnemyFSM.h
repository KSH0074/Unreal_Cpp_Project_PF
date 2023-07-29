// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

//���� State, �������Ʈ���� ��밡���ϰ� �Ͽ� �ִϸ޴ϼ� �������Ʈ �� BT������ �� �� �ֵ��� �� 
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	//Default, ����Ʈ�� Idle�� ��ü
	Idle,
	Patrol,
	Trace,
	Attack,
	TooFar,
	Damage,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_CPP_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
