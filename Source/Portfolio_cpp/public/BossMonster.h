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

	////피격함수 
	//void OnDamageProcess(int32 damage) override;

	////사망할때 작동하는 함수
	//void DeathState() override;

	////공격함수
	//void Attack() override;

	////공격판정함수 
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

	
	////공격범위변수
	//UPROPERTY(EditAnyWhere, Category = Attack)
	//	float fMeleeAttackRange = 120.0f;
};
