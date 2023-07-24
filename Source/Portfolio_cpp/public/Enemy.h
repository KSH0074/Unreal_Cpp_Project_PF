// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class PORTFOLIO_CPP_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	//this 대체를 위한 변수 
	UPROPERTY()
	class AEnemy* me;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

	//HP
	UPROPERTY(EditAnywhere)
	float HP = 100.0f;

	//피격함수 
	UFUNCTION()
	void OnDamageProcess(float damage);

	//사망할때 작동하는 함수
	UFUNCTION()
	void DeathState();

	//공격함수
	UFUNCTION()
	void AttackPlayer();


	//BlueprintReadWrite로  Enemy_Controller 블루프린트에서 접근하려했으나 실패함
	UPROPERTY(BlueprintReadWrite)
	class AMainPlayer* Scene_Placed_PlayerPawn;

	//공격범위변수
	UPROPERTY(EditAnyWhere, Category = Attack)
	float MeeleAttackRange = 120.0f;

	//공격 쿨타임
	UPROPERTY(EditAnyWhere, Category = Attack)
	float attackCoolTime = 1.0f;

	UPROPERTY()
	float currentTime{};
	
	//애니메이션 BP에서 스테이트 변경에 사용하기 위한 Bool변수, 추후 UENUM으로 관리할 필요가 있을 걸로 보임 => FSM 화 하여 BT연계 
	UPROPERTY(BlueprintReadOnly)
	bool isAttack = false;

	//사망여부 변수
	UPROPERTY(BlueprintReadOnly)
	bool isDead = false;

	//피격여부 변수
	UPROPERTY(BlueprintReadOnly)
		bool isDamaged = false;
};
