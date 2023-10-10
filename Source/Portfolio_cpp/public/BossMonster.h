// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "BossMonster.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_CPP_API ABossMonster : public AEnemy
{
	GENERATED_BODY()
	
public:
	ABossMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	//virtual void Tick(float DeltaTime);

	////�ǰ��Լ� 
	//void OnDamageProcess(int32 damage) override;

	////����Ҷ� �۵��ϴ� �Լ�
	//void DeathState() override;

	////�����Լ�
	//void Attack() override;

	////���������Լ� 
	//void attackZoneBeginOverlap(UPrimitiveComponent* OverlappedComp,
	//		AActor* OtherActor,
	//		UPrimitiveComponent* OtherComp,
	//		int32 OtherBodyIndex,
	//		bool bFromSweep,
	//		const FHitResult& SweepResult) override;

	//void attackZoneEndOverlap(
	//		UPrimitiveComponent* OverlappedComponent,
	//		AActor* OtherActor,
	//		UPrimitiveComponent* OtherComp,
	//		int32 OtherBodyIndex) override;

	
	////���ݹ�������
	//UPROPERTY(EditAnyWhere, Category = Attack)
	//	float fMeleeAttackRange = 120.0f;
};
