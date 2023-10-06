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

	mDamagedMontage = TempMontage.Object;
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> TempMontageFire(TEXT("AnimMontage'/Game/ImportedAnimationAndCharacter/Player/Player_Animation/Active/PlayerSkillMontage.PlayerSkillMontage'"));

	if (!TempMontageFire.Succeeded())
	{
		UE_LOG(Player, Warning, TEXT("Not found Montage"));
	}
	else
		UE_LOG(Player, Warning, TEXT("found Montage"));
	mSkillMontage = TempMontageFire.Object;
}

void UMainPlayerAnim::NativeBeginPlay()
{
	mMainPlayer = Cast<AMainPlayer>(TryGetPawnOwner());
}

//피격 관련 
void UMainPlayerAnim::PlayDamageMontage()
{
	//피격시 몽타주 재생
	//함수 매개변수를 다 채우기 
	Montage_Play(mDamagedMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
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

//fireBall
void UMainPlayerAnim::AnimNotify_FireBallfire()
{
	//발사하고 Input 활성화 
	mMainPlayer->ThrowFireball();
	mMainPlayer->AllowInput(true);
}


void UMainPlayerAnim::PlaySkillMontage(float playRate, const FName& MontageName)
{
	SkillSequence(playRate, MontageName);
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
	//공격판정 박스 오버랩 비활성화 
	mMainPlayer->AttackZoneControl(mMainPlayer->PlayerTempBox,false);
	mMainPlayer->MovementModeChange(mMainPlayer->GetCharacterMovement(), EMovementMode::MOVE_NavWalking);
}

void UMainPlayerAnim::AnimNotify_DodgeEnd()
{	
	UE_LOG(LogTemp, Warning, TEXT("DodgeEnd"));
	//AnimNotify_AttackEnd 와 거의 동일 AttackZoneControl의 매개변수하나만 다른 경우인데 이 함수를 이렇게 그냥 둬도 되나?
	mMainPlayer->AllowInput(true);
	//피격판정 박스 오버랩 비활성화 
	mMainPlayer->AttackZoneControl(mMainPlayer->PlayerTempBox, true);
	mMainPlayer->MovementModeChange(mMainPlayer->GetCharacterMovement(), EMovementMode::MOVE_NavWalking);
}

//스킬 발동시퀀스 
void UMainPlayerAnim::SkillSequence(float playRate, const FName& skillName)
{
	Montage_Play(mSkillMontage, playRate, EMontagePlayReturnType::MontageLength, 0.0f, true);
	Montage_JumpToSection(skillName, mSkillMontage);

	mMainPlayer->AllowInput(false);
	mMainPlayer->GetController()->StopMovement();
}
