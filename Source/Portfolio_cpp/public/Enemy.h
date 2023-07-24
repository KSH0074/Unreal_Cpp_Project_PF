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

	//this ��ü�� ���� ���� 
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

	//�ǰ��Լ� 
	UFUNCTION()
	void OnDamageProcess(float damage);

	//����Ҷ� �۵��ϴ� �Լ�
	UFUNCTION()
	void DeathState();

	//�����Լ�
	UFUNCTION()
	void AttackPlayer();


	//BlueprintReadWrite��  Enemy_Controller �������Ʈ���� �����Ϸ������� ������
	UPROPERTY(BlueprintReadWrite)
	class AMainPlayer* Scene_Placed_PlayerPawn;

	//���ݹ�������
	UPROPERTY(EditAnyWhere, Category = Attack)
	float MeeleAttackRange = 120.0f;

	//���� ��Ÿ��
	UPROPERTY(EditAnyWhere, Category = Attack)
	float attackCoolTime = 1.0f;

	UPROPERTY()
	float currentTime{};
	
	//�ִϸ��̼� BP���� ������Ʈ ���濡 ����ϱ� ���� Bool����, ���� UENUM���� ������ �ʿ䰡 ���� �ɷ� ���� => FSM ȭ �Ͽ� BT���� 
	UPROPERTY(BlueprintReadOnly)
	bool isAttack = false;

	//������� ����
	UPROPERTY(BlueprintReadOnly)
	bool isDead = false;

	//�ǰݿ��� ����
	UPROPERTY(BlueprintReadOnly)
		bool isDamaged = false;
};
