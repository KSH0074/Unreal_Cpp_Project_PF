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

	//AttackZone //�������� ����, �� ������Ʈ�� PlayerHitZone�� overlap�Ǿ��ְ� ���� �ִϸ��̼��� Notify�� ȣ��� �� �������� �ֵ��� ��
	UPROPERTY(EditAnywhere)
	class UBoxComponent* attackZoneComp;

	//HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP {};

	//�ǰ��Լ� 
	UFUNCTION()
		virtual	void OnDamageProcess(int32 damage);

	//����Ҷ� �۵��ϴ� �Լ�
	UFUNCTION()
		virtual void DeathState();

	//�����Լ�
	UFUNCTION()
		virtual void Attack();
	
	//���������Լ� 
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
	
	//���ݷ�
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 mDamage = 2;

	//BlueprintReadWrite��  Enemy_Controller �������Ʈ���� �����Ϸ������� ������
	UPROPERTY(BlueprintReadWrite)
	class AMainPlayer* Scene_Placed_PlayerPawn;

	//���ݹ�������
	UPROPERTY(EditAnyWhere, Category = Attack)
	float fMeleeAttackRange = 120.0f;


	


	//������� ����
	UPROPERTY(BlueprintReadOnly,category = "state")
	bool isDead = false;

	//�ǰݿ��� ����
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
