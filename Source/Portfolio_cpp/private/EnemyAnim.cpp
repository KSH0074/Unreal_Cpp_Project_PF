// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "MainPlayer.h"


void UEnemyAnim::AnimNotify_Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Called notify : Attack"));

	ownerIsEnemy = Cast<AEnemy>(TryGetPawnOwner());

	if (ownerIsEnemy == nullptr)
	{

		UE_LOG(LogTemp, Warning, TEXT("Called notify : Attack failed "));
	}

	if (ownerIsEnemy->bHit)
	{
	ownerIsEnemy->Scene_Placed_PlayerPawn->OnDamageProcess(ownerIsEnemy->mDamage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player aint HIt"));
	}
	
}

void UEnemyAnim::AnimNotify_OnAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Called notify : On Attack End"));
	// ������ ������ ���� BTT�� ������ �Ϸ��� �Ѵ�. ���⼭ ���� �� ������ �ֱ� ������ AttacktoPlayer���� return Successed�� ���� �ʵ��� �ϸ� ���  
	ownerIsEnemy->isMontagePlaying = false;
}

