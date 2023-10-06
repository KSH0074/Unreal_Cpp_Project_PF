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
	virtual void Tick(float DeltaTime) override;

};
