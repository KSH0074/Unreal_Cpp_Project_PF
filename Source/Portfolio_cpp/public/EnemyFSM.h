// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

//상태 State, 블루프린트에서 사용가능하게 하여 애니메니션 블루프린트 및 BT에서도 쓸 수 있도록 함 
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	//Default, 디폴트는 Idle로 대체
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
