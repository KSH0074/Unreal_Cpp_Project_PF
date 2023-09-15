// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerAnim.h"

#include "MainPlayer.h"


UMainPlayerAnim::UMainPlayerAnim()
{
	//생성자에서 ConstructorHelpers 를 쓰는 것 이름부터가 생성자도우미
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempMontage(TEXT("AnimMontage'/Game/ImportedAnimationAndCharacter/Player/Player_Animation/Active/Body_Block_Montage.Body_Block_Montage'"));

	if (!TempMontage.Succeeded())
	{
		UE_LOG(Player, Warning, TEXT("Not found Montage"));
	}
	else
		UE_LOG(Player, Warning, TEXT("found Montage"));

	damagedMontage = TempMontage.Object;
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempMontageFire(TEXT("AnimMontage'/Game/ImportedAnimationAndCharacter/Player/Player_Animation/Active/Fireball_Montage.Fireball_Montage'"));

	if (!TempMontageFire.Succeeded())
	{
		UE_LOG(Player, Warning, TEXT("Not found Montage"));
	}
	else
		UE_LOG(Player, Warning, TEXT("found Montage"));
	fireBallMontage = TempMontageFire.Object;
}

void UMainPlayerAnim::PlayDamageMontage()
{
	//함수 매개변수를 다 채우기 
	Montage_Play(damagedMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
}

void UMainPlayerAnim::NativeBeginPlay()
{
	mMainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());
}

void UMainPlayerAnim::AnimNotify_PlayerDamagedStart()
{
	UE_LOG(Player, Warning, TEXT("Player Damaged Start"));
	mMainPlayer->AllowInput(false);
	mMainPlayer-> GetController()->StopMovement();
}


void UMainPlayerAnim::AnimNotify_PlayerDamagedEnd()
{
	UE_LOG(Player, Warning, TEXT("Player Damaged End"));
	mMainPlayer->AllowInput(true);
}



//fire ball 
void UMainPlayerAnim::PlayFireBallMontage()
{
	Montage_Play(fireBallMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
	mMainPlayer->AllowInput(false);
	mMainPlayer->GetController()->StopMovement();
}

void UMainPlayerAnim::AnimNotify_FireBallfire()
{
	mMainPlayer->ThrowFireball();
	mMainPlayer->AllowInput(true);
}
