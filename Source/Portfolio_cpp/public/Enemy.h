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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//공격 범위
	//UPROPERTY(EditAnyWhere, Category = FSM)
	//float attackRange = 150.0f;

	//HP
	UPROPERTY(EditAnywhere)
	float HP = 1000.0f;

	//피격함수 
	UFUNCTION()
	void OnDamageProcess(float damage);
};
