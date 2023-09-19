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

	//Player ÇÇ°Ý
	UFUNCTION()
	void AnimNotify_PlayerDamagedStart();

	UFUNCTION()
	void AnimNotify_PlayerDamagedEnd();

	UFUNCTION()
	void PlayDamageMontage();

	//FireBall
	UFUNCTION()
	void PlayFireBallMontage();
	
	UFUNCTION()
	void AnimNotify_FireBallfire();

	//Hurricane Kick
	UFUNCTION()
	void PlayHurricaneMontage();

	//Attack
	UFUNCTION()
	void AnimNotify_Attack();
	
	UFUNCTION()
	void AnimNotify_AttackEnd();
	

	
	class AMainPlayer* mMainPlayer{};
	class UAnimMontage* mDamagedMontage{};
	class UAnimMontage* mSkillMontage{};

	UFUNCTION()
	void SkillSquence(float playRate,FName skillName);
	
};
