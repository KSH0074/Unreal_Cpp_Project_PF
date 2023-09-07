// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "MainPlayer.h"


void UEnemyAnim::AnimNotify_Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Called notify : Attack"));

	if (bHit)
	{
	ownerIsEnemy = Cast<AEnemy>(TryGetPawnOwner());
	ownerIsEnemy->Scene_Placed_PlayerPawn->OnDamageProcess(ownerIsEnemy->mDamage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player aint HIt"));
	}
	ownerIsEnemy->isMontagePlaying = true;
}

void UEnemyAnim::AnimNotify_OnAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Called notify : On Attack End"));
	// 공격이 끝나야 다음 BTT로 가도록 하려고 한다. 여기서 공격 끝 판정을 주기 전까지 AttacktoPlayer에서 return Successed를 하지 않도록 하면 어떨까  
	ownerIsEnemy->isMontagePlaying = false;
}
