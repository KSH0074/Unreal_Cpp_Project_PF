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

	//���⿡ �ִϸ��̼� ����� �����ϴ� ������ Enemy�κ��� �ű��.
	//true, false ���� ��ȭ�� �ִϸ��̼� ��Ƽ���̸� �̿��Ͽ� �ִϸ��̼��� ������ ������ ��ġ�Ѵ�. 
	//Enemy�� EnemyAnim�� include �Ͽ� ��Ÿ�� ����� ������ �� �ֵ����Ѵ�.

	//���� �ִϸ��̼� ��� �Լ� ��������ͷ� ���� => BP_Enemy�� ������ �����ϴ�. "BlueprintImplementableEvent""
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttackAnim(FName sectionName);


};
