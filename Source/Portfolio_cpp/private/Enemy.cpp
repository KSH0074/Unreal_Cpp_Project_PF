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

	//EnemyŬ������ ��� �ֹ̳��̼� �������Ʈ, ���̷�Ż �޽� �Ҵ� 
	if (GetClass()->GetName() == FString("Enemy"))
	{
		ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("AnimBlueprint'/Game/ImportedAnimationAndCharacter/Enemy/Enemy_Animation/Enemy_AnimBP.Enemy_AnimBP_C'"));

		if (tempClass.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(tempClass.Class);
		}


		ConstructorHelpers::FObjectFinder < USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/ImportedAnimationAndCharacter/Enemy/Mesh/Enemy_Skeletalmesh.Enemy_Skeletalmesh'"));

		if (tempMesh.Succeeded())
		{
			GetMesh()->SetSkeletalMesh(tempMesh.Object);

			GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
		}
		UE_LOG(LogTemp, Warning, TEXT("Enemy Initialize"));

		ConstructorHelpers::FClassFinder <AAIController> tempAIClass(TEXT("Blueprint'/Game/Blueprints/Enemy_AI/Enemy_Controller.Enemy_Controller_C'"));
		if (tempAIClass.Succeeded())
		{
			AIControllerClass = tempAIClass.Class;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Faild give Default AIclass to Boss"));
		}
	}
	

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel3);

	attackZoneComp = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackZone"));
	attackZoneComp->SetupAttachment(RootComponent);

	attackZoneComp->SetRelativeLocation(FVector(70.0f, 0.0f, 0.0f), false, nullptr, ETeleportType::None);
	attackZoneComp->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.5f));
	attackZoneComp->SetGenerateOverlapEvents(true);
	attackZoneComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	attackZoneComp->SetCollisionObjectType(ECC_GameTraceChannel7);//Enemy AttackZone
	attackZoneComp->bHiddenInGame = false;
	//����ä�� ����
	attackZoneComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	attackZoneComp->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECollisionResponse::ECR_Overlap); // Player Hit Box

	//attackZoneComp �� BeginOverlap������ ��������Ʈ 
	attackZoneComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::attackZoneBeginOverlap);
	attackZoneComp->OnComponentEndOverlap.AddDynamic(this, &AEnemy::attackZoneEndOverlap);  //Overlap�� ������ �� ����� �ص� ���������� ���� �ʵ��� 


	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	HP = 100;
	
	mDamage = 2;
	
}


// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	anim = Cast<UEnemyAnim>(GetMesh()->GetAnimInstance());
	mController = Cast<AEnemyAIController>(GetController());
	Scene_Placed_PlayerPawn = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));
	UE_LOG(LogTemp, Warning, TEXT("Test %s : "), *Scene_Placed_PlayerPawn->GetName());
	fMeleeAttackRange = 120.0f;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�÷��̾� ��ġ ������Ʈ Player Location Update
	FVector PlayerLocation = Scene_Placed_PlayerPawn->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s is Enemy recognize playerFVector"),*PlayerLocation.ToString());

	//�÷��̾�� �Ÿ��� ���ݹ��� ���Ϸ� �������� 
	 distance = PlayerLocation - GetActorLocation();

	/*
	 IsAttack �� �ƴϰ� ����ﶧ true // IsAttack�� Default�� False
	 IsAttack �̸鼭 ������ ������ False
	 if ����ϴ� ���� ���� ���α׷����� �� �� �ƴ»������ �� �� �ִٴµ�
	 �� if���� �ּ��ϱ�?
	*/
	if (distance.Size() < fMeleeAttackRange && !(mController->getBlackBoardState("IsAttack")))
	{
		//UE_LOG(LogTemp, Warning, TEXT("parent call Distance: %f"), distance.Size());
		mController->ChangeBlackBoardState(EEnemyState::Attack, true);

	}
	//isAttack ���¿��� ���� ���ݺ��� �־����鼭 ��Ÿ�ְ� �÷������� �ƴҶ� ���º�ȭ 
	else if (mController->getBlackBoardState("IsAttack") && distance.Size() > fMeleeAttackRange && !isMontagePlaying)
	{
		mController->ChangeBlackBoardState(EEnemyState::Attack, false);
		//UE_LOG(LogTemp, Warning, TEXT("Change State  IsAttack false"));
	}
}




void AEnemy::OnDamageProcess(int32 damage)
{
	//BehaviorTree �����ٷ� �÷��������ϵ��� �� 
	mController->BlackboardIsDamagedSet(true);


	//�������� �Ծ��� ��� ���ݻ��� ����
	mController->ChangeBlackBoardState(EEnemyState::Attack, false);
	isMontagePlaying = false;


	HP -= damage;



	UE_LOG(LogTemp, Warning, TEXT("Enemy HP: % d"), HP);
	if (HP <= 0)
	{
		isDead = true;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mController->ChangeBlackBoardState(EEnemyState::Die, true);
	}


}
void AEnemy::DeathState()
{
	//p = p0 + vt
	FVector p0 = GetActorLocation();
	FVector vt = FVector::DownVector * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + (40.0f * vt);
	SetActorLocation(p);
	if (p.Z < -200.0f)
	{
		Destroy();
	}

}

void AEnemy::Attack()
{

	int32 index = FMath::RandRange(0.0f, 1.9f);
	FString sectionName = FString::Printf(TEXT("Attack%d"), index);
	anim->PlayAttackAnim(FName(*sectionName));//BP���� ������ �Լ��� ����� 



	isMontagePlaying = true;
	GetController()->StopMovement();

	UE_LOG(LogTemp, Warning, TEXT("Enemy's Attack! %d"), index);

}

void AEnemy::attackZoneBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//Overlap�� OtherComponent�� PlayerHitZone���� if �� Ȯ�� ��, ���� ��� ��Ƽ���̿��� Player�� OnDamageProcess �Լ��� ����
	//���� ���⿡�� PlayerHitZone �� �´����� Ȯ���ϰ� �´ٸ� true ���� ��Ƽ���̿� ����, ��Ƽ���̿����� true�ϰ�� OnDamageProcess ���� 
	if (OtherComp->GetName() == FString("HitBox"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped Component : %s "), *(OtherComp->GetFName().ToString()));
	}
	bHit = true;

}

void AEnemy::attackZoneEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bHit = false;
}


//void AEnemy::getMontageSectionPlaytime(int32 index)
//{
//	//��Ÿ�� ����Ҷ� ���� ȣ��Ǵ� ������ �ִ�. ����ǰ��ִ� ��Ÿ���� �÷���Ÿ���� �˰���� �ʴٸ�  ������ �Ǵ� BeginPlay���� �˰���� ��Ÿ�ָ� ������ �ʱ�ȭ �� �� �ֵ��� ��ġ�� ���� ���� �� 
//	//�ٸ� ������ �������ٸ� �̴�� ���δ°� ���� �� �ִ�.
//
//	UAnimMontage* montage = anim->GetCurrentActiveMontage();
//	//float test = PlayAnimMontage(montage, 1.0f, montageSectionName); ����° �Ű������� ������ ������ �ǹ̾��� ���� ����� ������� ��Ÿ�� ��ü������̸� ��ȯ 
//	float StartTime{};
//	float EndTime{};
//	//UE_LOG(LogTemp, Warning, TEXT("%s Montage playTime is : % f"), *montageSectionName.ToString(), test); ���� ����� ������� ��Ÿ�� ��ü������̸� ��ȯ 
//	montage->GetSectionStartAndEndTime(index, StartTime, EndTime);
//	//float test = EndTime - StartTime; ���� �� ���� ��Ȯ�� �÷���Ÿ��, float �ڷ����� ������ �Ҽ��� ���� ����  
//	fMontageSectionPlaytime = EndTime - StartTime;
//	UE_LOG(LogTemp, Warning, TEXT("Attack %d  Montage playTime is : % f"), index, fMontageSectionPlaytime);
//
//}
