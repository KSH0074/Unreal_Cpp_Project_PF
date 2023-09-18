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
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempMontageFire(TEXT("AnimMontage'/Game/ImportedAnimationAndCharacter/Player/Player_Animation/Active/PlayerSkillMontage.PlayerSkillMontage'"));

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
	Montage_Play(damagedMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, false);
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
	Montage_JumpToSection("FireBall", fireBallMontage);

	mMainPlayer->AllowInput(false);
	mMainPlayer->GetController()->StopMovement();

	
}

void UMainPlayerAnim::AnimNotify_FireBallfire()
{
	mMainPlayer->ThrowFireball();
	mMainPlayer->AllowInput(true);
}

//Hurricane Kick 섹션이름 뺴고 PlayFireBallMontage와 코드가 같다. 함수화 가능 
void UMainPlayerAnim::PlayHurricaneMontage()
{
	Montage_Play(fireBallMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
	Montage_JumpToSection(FName("HurricaneKick"), fireBallMontage);

	mMainPlayer->AllowInput(false);
	mMainPlayer->GetController()->StopMovement();
}


//Attack 대부분의 물리형 공격에서 쓰일 노티파이 
void UMainPlayerAnim::AnimNotify_Attack()
{

	if (mMainPlayer->bHit)
	{
		mMainPlayer->mHittedEnemy->OnDamageProcess(mMainPlayer->mPlayerPower);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy aint HIt"));
	}

}

void UMainPlayerAnim::AnimNotify_AttackEnd()
{
	mMainPlayer->AllowInput(true);
}
