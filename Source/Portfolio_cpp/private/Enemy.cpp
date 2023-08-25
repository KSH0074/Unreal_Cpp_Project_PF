 // Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include <Components/CapsuleComponent.h>
#include "MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h" 
#include "EnemyAnim.h"
#include "EnemyAIController.h"
#include <Components/BoxComponent.h>
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder < USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/ImportedAnimationAndCharacter/Enemy/Mesh/Enemy_Skeletalmesh.Enemy_Skeletalmesh'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel3);

	//애미네이션 블루프린트 할당 
	ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("AnimBlueprint'/Game/ImportedAnimationAndCharacter/Enemy/Enemy_Animation/Enemy_AnimBP.Enemy_AnimBP_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}
	


}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//GetPlayerPawn으로 대체해도 무방함 
	Scene_Placed_PlayerPawn = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));
	anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	mController = Cast<AEnemyAIController>(GetController());
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	//관련 및 비슷한 부분 전부 테스트코드이며 테스트 완성후 FSM 또는 BT로 편입, DeathState, AttackPlayer 전부 BlackBoard에 관련 키를 추가하고 BT에서 판단하여 작동하도록 이 부분을 수정, 
	if (isDead)
	{
		mController->ChangeBlackBoardState(EEnemyState::Die,true);
	}

	//플레이어 위치 업데이트 Player Location Update
	FVector PlayerLocation = Scene_Placed_PlayerPawn->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s is Enemy recognize playerFVector"),*PlayerLocation.ToString());

	//플레이어와 거리가 공격범위 이하로 내려가면 
	FVector distance = PlayerLocation - GetActorLocation();

	/*
	 IsAttack 이 아니고 가까울때 true // IsAttack은 Default가 False
	 IsAttack 이면서 가깝지 않을때 False
	 if 사용하는 법만 봐도 프로그래밍을 할 줄 아는사람인지 알 수 있다는데 
	 이 if문은 최선인가?
	*/
	if (distance.Size() < MeeleAttackRange)
	{
			mController->ChangeBlackBoardState(EEnemyState::Attack, true);
	
	}
	else if(mController->getBlackBoardState("IsAttack") &&  !(distance.Size() < MeeleAttackRange))
	{
		anim->Montage_Stop(0.0f);//애니메이션 몽타주 즉시 중지 when Attack is Flase

		mController->ChangeBlackBoardState(EEnemyState::Attack, false);

	}
}




void AEnemy::OnDamageProcess(int damage)
{
	//isDamaged = true;

	//AIController Get 해서 BB의 Is Damaged  true 


		//BehaviorTree 에서바로 플레이추적하도록 함 
		mController->BlackboardIsDamagedSet(true);
	

	
	HP-= damage;
	UE_LOG(LogTemp, Warning, TEXT("HP:%d"),HP );
	if (HP <= 0)
	{
		isDead = true;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	

}
void AEnemy::DeathState()
{
	//p = p0 + vt
	FVector p0 = GetActorLocation();
	FVector vt = FVector::DownVector * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + (30.0f*vt);
	SetActorLocation(p);
	if (p.Z < -200.0f)
	{
		Destroy();
	}
	
}

void AEnemy::AttacktoPlayer()
{

	//UE_LOG(LogTemp, Warning, TEXT("CurrentTime is : %f"), currentTime);

	if (currentTime >= mAttackCoolTime)
	{
		
		int32 index = FMath::RandRange(0.0f, 1.9f);
		FString sectionName = FString::Printf(TEXT("Attack%d"), index);
		anim->PlayAttackAnim(FName(*sectionName));//BP에서 구현된 함수가 실행됨 
		UE_LOG(LogTemp, Warning, TEXT("Enemy's Attack! %d"), index);
		currentTime = 0.5f;
	}
		currentTime = currentTime + GetWorld()->DeltaTimeSeconds;
}
