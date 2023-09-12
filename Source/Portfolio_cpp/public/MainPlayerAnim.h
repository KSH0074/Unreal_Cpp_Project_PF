// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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
//	virtual void NativeBeginPlay() override;

public:
	UFUNCTION()
	void AnimNotify_PlayerDamagedStart();

	UFUNCTION()
	void AnimNotify_PlayerDamagedEnd();

	UFUNCTION()
	void PlayDamageMontage();
	


	class UAnimMontage* damagedMontage{};
	
};
