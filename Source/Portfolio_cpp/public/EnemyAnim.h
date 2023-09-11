// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_CPP_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	class UAnimMontage* UanimAttack;

	//UPROPERTY()
	class AEnemy* ownerIsEnemy;

	//Attack Notify Execute
	UFUNCTION()
	void AnimNotify_Attack();
	
	//OnAttackEnd Notify Execute
	UFUNCTION()
	void AnimNotify_OnAttackEnd();
	//hit 
	
	
	//Enemy가 EnemyAnim을 include 하여 몽타주 재생에 관여할 수 있도록한다.

	//공격 애니메이션 재생 함수 블루프린터로 구현 => BP_Enemy에 접근이 용이하다. "BlueprintImplementableEvent""
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttackAnim(FName sectionName);

};
