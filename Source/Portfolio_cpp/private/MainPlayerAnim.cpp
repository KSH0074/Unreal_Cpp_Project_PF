// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerAnim.h"
#include "Portfolio_cpp.h"
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

	damagedMontage = TempMontage.Object;
	UE_LOG(Player, Warning, TEXT("Montage Name : %s"), *damagedMontage->GetFName().ToString());
	//TryGetPawnOwner()
}



void UMainPlayerAnim::AnimNotify_PlayerDamagedStart()
{
	UE_LOG(Player, Warning, TEXT("Player Damaged Start"));
	
}


void UMainPlayerAnim::AnimNotify_PlayerDamagedEnd()
{
	UE_LOG(Player, Warning, TEXT("Player Damaged End"));
}

void UMainPlayerAnim::PlayDamageMontage()
{
	//�Լ� �Ű������� �� ä��� 
	Montage_Play(damagedMontage,1.0f,EMontagePlayReturnType::MontageLength,0.0f,true);
}