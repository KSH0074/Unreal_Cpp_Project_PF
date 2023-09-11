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
	
	
	//Enemy�� EnemyAnim�� include �Ͽ� ��Ÿ�� ����� ������ �� �ֵ����Ѵ�.

	//���� �ִϸ��̼� ��� �Լ� ��������ͷ� ���� => BP_Enemy�� ������ �����ϴ�. "BlueprintImplementableEvent""
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttackAnim(FName sectionName);

};
