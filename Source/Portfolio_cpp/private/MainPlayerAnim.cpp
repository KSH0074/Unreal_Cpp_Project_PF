// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerAnim.h"

#include "MainPlayer.h"


UMainPlayerAnim::UMainPlayerAnim()
{
	//�����ڿ��� ConstructorHelpers �� ���� �� �̸����Ͱ� �����ڵ����
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

//������ ���� 
void UMainPlayerAnim::PlayDamageMontage()
{
	//�ǰݽ� ��Ÿ�� ���
	//�Լ� �Ű������� �� ä��� 
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
	//�߻��ϰ� Input Ȱ��ȭ 
	mMainPlayer->ThrowFireball();
	mMainPlayer->AllowInput(true);
}

//fire ball 
//void UMainPlayerAnim::PlayFireBallMontage()
//{	
//	SkillSquence(1.5f, "FireBall");
//}
//
////Hurricane Kick �����̸� ���� PlayFireBallMontage�� �ڵ尡 ����. �Լ�ȭ ���� 
//void UMainPlayerAnim::PlayHurricaneMontage()
//{
//	SkillSquence(0.8f, "HurricaneKick");
//}
//
//void UMainPlayerAnim::PlayNormailAttackMontage()
//{
//	SkillSquence(1.0f, "NormalAttack");
//}
//
//void UMainPlayerAnim::PlayFlyingKickMontage()
//{
//	SkillSquence(1.0f, "FlyingKick");
//}


void UMainPlayerAnim::PlaySkillMontage(float playRate, const FName& MontageName)
{
	SkillSequence(playRate, MontageName);
}


//Attack ��κ��� ������ ���ݿ��� ���� ��Ƽ���� 
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
	//�������� �ڽ� ������ ��Ȱ��ȭ 
	mMainPlayer->AttackZoneControl(mMainPlayer->PlayerTempBox,false);
}

//��ų �ߵ������� 
void UMainPlayerAnim::SkillSequence(float playRate, const FName& skillName)
{
	Montage_Play(mSkillMontage, playRate, EMontagePlayReturnType::MontageLength, 0.0f, true);
	Montage_JumpToSection(skillName, mSkillMontage);

	mMainPlayer->AllowInput(false);
	mMainPlayer->GetController()->StopMovement();
}
