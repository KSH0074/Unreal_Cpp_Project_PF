// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"


void UEnemyAnim::AnimNotify_Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Called notify : Attack"));

	ownerIsEnemy = Cast<AEnemy>(TryGetPawnOwner());
	UE_LOG(LogTemp, Warning, TEXT("Owner is %s"), *(ownerIsEnemy->GetFName().ToString()));
}

void UEnemyAnim::AnimNotify_OnAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Called notify : On Attack End"));

}
