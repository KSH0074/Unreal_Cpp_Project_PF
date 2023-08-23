// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

// C++ 로 작업하게 되면 굉장히 길고 구현이 번거로우므로 블루프린트로 구현한다.
//void UEnemyAnim::PlayAttackAnim(FName sectionName)
//{
//	ConstructorHelpers::FObjectFinder<UAnimMontage> tempAnim(TEXT("AnimMontage'/Game/ImportedAnimationAndCharacter/Enemy/Enemy_Animation/Active/Attack_montage.Attack_montage'"));
//	....
//	Cast<AEnemy>(TryGetPawnOwner())->PlayAnimMontage()
//}

void UEnemyAnim::AnimNotify_Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Called notify : Attack"));
}

void UEnemyAnim::AnimNotify_OnAttackEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Called notify : On Attack End"));

}
