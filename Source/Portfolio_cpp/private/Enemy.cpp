 // Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include <Components/CapsuleComponent.h>
#include "MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h" 
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
	

}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//GetPlayerPawn으로 대체해도 무방함 
	Scene_Placed_PlayerPawn = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));
	anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	Controller = Cast<AEnemyAIController>(GetController());
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	//관련 및 비슷한 부분 전부 테스트코드이며 테스트 완성후 FSM 또는 BT로 편입, DeathState, AttackPlayer 전부 BlackBoard에 관련 키를 추가하고 BT에서 판단하여 작동하도록 이 부분을 수정, 
	if (isDead)
	{
		Controller->ChangeBlaockBoardState(EEnemyState::Die,true);
	}

	//플레이어 위치 업데이트 Player Location Update
	FVector PlayerLocation = Scene_Placed_PlayerPawn->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s is Enemy recognize playerFVector"),*PlayerLocation.ToString());

	//플레이어와 거리가 공격범위 이하로 내려가면 
	FVector distance = PlayerLocation - GetActorLocation();
	if (distance.Size() < MeeleAttackRange)
	{
		Controller->ChangeBlaockBoardState(EEnemyState::Attack, true);
	}
	else
	{
		anim->Montage_Stop(0.0f);//애니메이션 몽타주 즉시 중지 when Attack is Flase

		Controller->ChangeBlaockBoardState(EEnemyState::Attack, false); 

	}
}




void AEnemy::OnDamageProcess(int damage)
{
	//isDamaged = true;

	//AIController Get 해서 BB의 Is Damaged  true 
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController != nullptr)
	{
		//BehaviorTree 에서바로 플레이추적하도록 함 
		AIController->BlackboardIsDamagedSet(true);
	}

	
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
	FVector tagetLocation = Scene_Placed_PlayerPawn->GetActorLocation();

	FVector LoockDirection = tagetLocation - GetActorLocation();
	
	//Player가 감지되지 않는 뒤 시야에서도 가까워지면 공격하도록 한다, SetActorRotation으로 하면 Controller는 회전하지 않기때문에 GetController를 사용해서 콘트롤러를 회전 

	GetController()->SetControlRotation(LoockDirection.Rotation());


	UE_LOG(LogTemp, Warning, TEXT("CurrentTime is : %f"), currentTime);
	//AttackPlayer가 
	if (currentTime >= mAttackCoolTime)
	{
		
		int32 index = FMath::RandRange(0.0f, 1.9f);
		FString sectionName = FString::Printf(TEXT("Attack%d"), index);
		anim->PlayAttackAnim(FName(*sectionName));//BP에서 구현된 함수가 실행됨 
		UE_LOG(LogTemp, Warning, TEXT("Enemy's Attack! %d"), index);
		currentTime = 0;
	}
		currentTime = currentTime + GetWorld()->DeltaTimeSeconds;
}
