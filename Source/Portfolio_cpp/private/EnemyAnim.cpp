// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

// C++ �� �۾��ϰ� �Ǹ� ������ ��� ������ ���ŷο�Ƿ� �������Ʈ�� �����Ѵ�.
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
