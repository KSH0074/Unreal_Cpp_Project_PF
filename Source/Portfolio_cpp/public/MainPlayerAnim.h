// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Portfolio_cpp.h"
#include "MainPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_CPP_API UMainPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMainPlayerAnim();
protected:
	virtual void NativeBeginPlay() override;

public:

	//Player �ǰ�
	UFUNCTION()
	void AnimNotify_PlayerDamagedStart();

	UFUNCTION()
	void AnimNotify_PlayerDamagedEnd();

	UFUNCTION()
	void PlayDamageMontage();

	//FireBall ����ü �߻� ��Ƽ����
	UFUNCTION()
	void AnimNotify_FireBallfire();


	
	//��ų ���� �� �Լ� ����ϸ� ��Ÿ�� ��� 
	UFUNCTION()
	void PlaySkillMontage(float PlayRate, const FName& MontageName);
	
	
	//Attack
	UFUNCTION()
	void AnimNotify_Attack();
	
	UFUNCTION()
	void AnimNotify_AttackEnd();
	

	
	class AMainPlayer* mMainPlayer{};
	class UAnimMontage* mDamagedMontage{};
	class UAnimMontage* mSkillMontage{};

	UFUNCTION()
	void SkillSequence(float playRate, const FName& skillName);
	
};
