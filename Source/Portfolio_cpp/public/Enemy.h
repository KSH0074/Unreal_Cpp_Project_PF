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



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Controller
	class AEnemyAIController* Controller;

	//HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP = 100;

	//피격함수 
	UFUNCTION()
	void OnDamageProcess(int damage);

	//사망할때 작동하는 함수
	UFUNCTION()
	void DeathState();

	//공격함수
	UFUNCTION()
	void AttackPlayer();
	
	//공격력
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int mDamage = 2;

	//BlueprintReadWrite로  Enemy_Controller 블루프린트에서 접근하려했으나 실패함
	UPROPERTY(BlueprintReadWrite)
	class AMainPlayer* Scene_Placed_PlayerPawn;

	//공격범위변수
	UPROPERTY(EditAnyWhere, Category = Attack)
	float MeeleAttackRange = 120.0f;

	//공격 쿨타임
	UPROPERTY(EditAnyWhere, Category = Attack)
	float mAttackCoolTime = 1.0f;


	//애니메이션 BP에서 스테이트 변경에 사용하기 위한 Bool변수, 추후 UENUM으로 관리할 필요가 있을 걸로 보임 => FSM 화 하여 BT연계 
	UPROPERTY(BlueprintReadOnly)
		bool isAttack = false;

	UPROPERTY()
	float currentTime{};
	


	//사망여부 변수
	UPROPERTY(BlueprintReadOnly)
	bool isDead = false;

	//피격여부 변수
	UPROPERTY(BlueprintReadOnly)
		bool isDamaged = false;
	
	UPROPERTY()
	class UEnemyAnim* anim;
};
