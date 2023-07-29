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

	//�ֹ̳��̼� �������Ʈ �Ҵ� 
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

	//GetPlayerPawn���� ��ü�ص� ������ 
	Scene_Placed_PlayerPawn = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	//���� �� ����� �κ� ���� �׽�Ʈ�ڵ��̸� �׽�Ʈ �ϼ��� FSM �Ǵ� BT�� ����, DeathState, AttackPlayer ���� BlackBoard�� ���� Ű�� �߰��ϰ� BT���� �Ǵ��Ͽ� �۵��ϵ��� �� �κ��� ����, 
	if (isDead)
	{
		DeathState();
	}

	//�÷��̾� ��ġ ������Ʈ Player Location Update
	FVector PlayerLocation = Scene_Placed_PlayerPawn->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s is Enemy recognize playerFVector"),*PlayerLocation.ToString());

	//�÷��̾�� �Ÿ��� ���ݹ��� ���Ϸ� �������� 
	FVector distance = PlayerLocation - me->GetActorLocation();
	if (distance.Size() < MeeleAttackRange)
	{
		isAttack = true;//���º�ȭ�� �����Ѵ�. ���� �Ʒ� �Լ��� ����ȴ�.
	
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

	//AIController Get �ؼ� BB�� Is Damaged  true �� �ϰ� Blueprint���� moveto FirstLocation �Ҷ� False �� �ٲٴ� �ڵ�
	AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
	if (AIController != nullptr)
	{
		//BehaviorTree �����ٷ� �÷��������ϵ��� �� 
		AIController->BlackboardIsDamagedSet(true);
	}
	//�ǰݾִϸ�Ÿ�� ��� 
	// 
	//ü�� ���� 
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
	
	//Player�� �������� �ʴ� �� �þ߿����� ��������� �����ϵ���, SetActorRotation���� �ϸ� Controller�� ȸ������ �ʱ⶧����  

	me->GetController()->SetControlRotation(LoockDirection.Rotation());

	currentTime = currentTime + GetWorld()->DeltaTimeSeconds;

	if (currentTime > mAttackCoolTime)
	{
		//anim->bAttackPlay = true;
		int32 index = FMath::RandRange(0.0f, 1.9f);
		FString sectionName = FString::Printf(TEXT("Attack%d"), index);
		anim->PlayAttackAnim(FName(*sectionName));//BP���� ������ �Լ��� ����� 
		UE_LOG(LogTemp, Warning, TEXT("Enemy's Attack! %d"), index);
		currentTime = 0;
	}
	
}
