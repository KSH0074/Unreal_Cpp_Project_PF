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
	class AEnemyAIController* mController;

	//AttackZone //공격판정 구역, 이 컴포넌트와 PlayerHitZone이 overlap되어있고 공격 애니메이션의 Notify가 호출될 때 데미지를 주도록 함
	UPROPERTY(EditAnywhere)
	class UBoxComponent* attackZoneComp;

	//HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP {};

	//피격함수 
	UFUNCTION()
		virtual	void OnDamageProcess(int32 damage);

	//사망할때 작동하는 함수
	UFUNCTION()
		virtual void DeathState();

	//공격함수
	UFUNCTION()
		virtual void Attack();
	
	//공격판정함수 
	UFUNCTION()
		virtual void attackZoneBeginOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
		virtual void attackZoneEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex);
	
	//공격력
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 mDamage = 2;

	//BlueprintReadWrite로  Enemy_Controller 블루프린트에서 접근하려했으나 실패함
	UPROPERTY(BlueprintReadWrite)
	class AMainPlayer* Scene_Placed_PlayerPawn;

	//공격범위변수
	UPROPERTY(EditAnyWhere, Category = Attack)
	float fMeleeAttackRange = 120.0f;


	


	//사망여부 변수
	UPROPERTY(BlueprintReadOnly,category = "state")
	bool isDead = false;

	//피격여부 변수
	UPROPERTY(BlueprintReadOnly)
		bool isDamaged = false;
	
	UPROPERTY()
	class UEnemyAnim* anim;

	bool isMontagePlaying = false;

	UPROPERTY()
	float fMontageSectionPlaytime{};

	UFUNCTION()
	void KnockBack(float knocbackLength,bool tof);

	UPROPERTY()
	bool bHit = false;

};
