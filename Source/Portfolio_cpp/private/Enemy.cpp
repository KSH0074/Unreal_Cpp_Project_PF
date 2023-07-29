 // Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include <Components/CapsuleComponent.h>
#include "MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAnim.h"
#include "EnemyAIController.h"
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
	me = this;
	
}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//GetPlayerPawn으로 대체해도 무방함 
	Scene_Placed_PlayerPawn = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	//관련 및 비슷한 부분 전부 테스트코드이며 테스트 완성후 FSM 또는 BT로 편입, DeathState, AttackPlayer 전부 BlackBoard에 관련 키를 추가하고 BT에서 판단하여 작동하도록 이 부분을 수정, 
	if (isDead)
	{
		DeathState();
	}

	//플레이어 위치 업데이트 Player Location Update
	FVector PlayerLocation = Scene_Placed_PlayerPawn->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s is Enemy recognize playerFVector"),*PlayerLocation.ToString());

	//플레이어와 거리가 공격범위 이하로 내려가면 
	FVector distance = PlayerLocation - me->GetActorLocation();
	if (distance.Size() < MeeleAttackRange)
	{
		isAttack = true;//상태변화라 가정한다. 추후 아래 함수로 흡수된다.
	
		AttackPlayer();
	}
	else
		isAttack = false; 
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnDamageProcess(float damage)
{
	//isDamaged = true;

	//AIController Get 해서 BB의 Is Damaged  true 로 하고 Blueprint에서 moveto FirstLocation 할때 False 로 바꾸는 코드
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController != nullptr)
	{
		//BehaviorTree 에서바로 플레이추적하도록 함 
		AIController->BlackboardIsDamagedSet(true);
	}
	//피격애니몽타주 재생 
	// 
	//체력 감소 
	HP -= damage;
	UE_LOG(LogTemp, Warning, TEXT("HP:%.2f"),HP );
	if (HP <= 0.0f)
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

void AEnemy::AttackPlayer()
{
	FVector tagetLocation = Scene_Placed_PlayerPawn->GetActorLocation();

	FVector LoockDirection = tagetLocation - me->GetActorLocation();
	
	//Player가 감지되지 않는 뒤 시야에서도 가까워지면 공격하도록, SetActorRotation으로 하면 Controller는 회전하지 않기때문에  

	me->GetController()->SetControlRotation(LoockDirection.Rotation());

	currentTime = currentTime + GetWorld()->DeltaTimeSeconds;

	if (currentTime > mAttackCoolTime)
	{
		//anim->bAttackPlay = true;
		int32 index = FMath::RandRange(0.0f, 1.9f);
		FString sectionName = FString::Printf(TEXT("Attack%d"), index);
		anim->PlayAttackAnim(FName(*sectionName));//BP에서 구현된 함수가 실행됨 
		UE_LOG(LogTemp, Warning, TEXT("Enemy's Attack! %d"), index);
		currentTime = 0;
	}
	
}
