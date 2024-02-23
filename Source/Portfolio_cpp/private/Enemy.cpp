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

	//Enemy클래스일 경우 애미네이션 블루프린트, 스켈레탈 메시 할당 
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
	//반응채널 설정
	attackZoneComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	attackZoneComp->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECollisionResponse::ECR_Overlap); // Player Hit Box

	//attackZoneComp 의 BeginOverlap에대한 델리게이트 
	attackZoneComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::attackZoneBeginOverlap);
	attackZoneComp->OnComponentEndOverlap.AddDynamic(this, &AEnemy::attackZoneEndOverlap);  //Overlap이 끝났을 때 헛발질 해도 공격판정이 되지 않도록 


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

	//플레이어 위치 업데이트 Player Location Update
	FVector PlayerLocation = Scene_Placed_PlayerPawn->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s is Enemy recognize playerFVector"),*PlayerLocation.ToString());

	//플레이어와 거리가 공격범위 이하로 내려가면 
	 distance = PlayerLocation - GetActorLocation();

	/*
	 IsAttack 이 아니고 가까울때 true // IsAttack은 Default가 False
	 IsAttack 이면서 가깝지 않을때 False
	 if 사용하는 법만 봐도 프로그래밍을 할 줄 아는사람인지 알 수 있다는데
	 이 if문은 최선일까?
	*/
	if (distance.Size() < fMeleeAttackRange && !(mController->getBlackBoardState("IsAttack")))
	{
		//UE_LOG(LogTemp, Warning, TEXT("parent call Distance: %f"), distance.Size());
		mController->ChangeBlackBoardState(EEnemyState::Attack, true);

	}
	//isAttack 상태에서 근접 공격보다 멀어지면서 몽타주가 플레이중이 아닐때 상태변화 
	else if (mController->getBlackBoardState("IsAttack") && distance.Size() > fMeleeAttackRange && !isMontagePlaying)
	{
		mController->ChangeBlackBoardState(EEnemyState::Attack, false);
		//UE_LOG(LogTemp, Warning, TEXT("Change State  IsAttack false"));
	}
}




void AEnemy::OnDamageProcess(int32 damage)
{
	//BehaviorTree 에서바로 플레이추적하도록 함 
	mController->BlackboardIsDamagedSet(true);


	//데미지를 입었을 경우 공격상태 해제
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
	anim->PlayAttackAnim(FName(*sectionName));//BP에서 구현된 함수가 실행됨 



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
	//Overlap된 OtherComponent가 PlayerHitZone인지 if 로 확인 후, 맞을 경우 노티파이에서 Player의 OnDamageProcess 함수를 실행
	//따라서 여기에는 PlayerHitZone 가 맞는지만 확인하고 맞다면 true 값을 노티파이에 전달, 노티파이에서는 true일경우 OnDamageProcess 실행 
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
//	//몽타주 재생할때 마다 호출되는 문제가 있다. 재생되고있는 몽타주의 플레이타임을 알고싶지 않다면  생성자 또는 BeginPlay에서 알고싶은 몽타주만 변수에 초기화 할 수 있도록 고치는 편이 나을 것 
//	//다만 섹션이 많아진다면 이대로 놔두는게 나을 수 있다.
//
//	UAnimMontage* montage = anim->GetCurrentActiveMontage();
//	//float test = PlayAnimMontage(montage, 1.0f, montageSectionName); 세번째 매개변수로 섹션이 들어가지만 의미없음 섹션 내용과 관계없이 몽타주 전체재생길이를 반환 
//	float StartTime{};
//	float EndTime{};
//	//UE_LOG(LogTemp, Warning, TEXT("%s Montage playTime is : % f"), *montageSectionName.ToString(), test); 섹션 내용과 관계없이 몽타주 전체재생길이를 반환 
//	montage->GetSectionStartAndEndTime(index, StartTime, EndTime);
//	//float test = EndTime - StartTime; 섹션 별 비교적 정확한 플레이타임, float 자료형이 가지는 소수점 오차 존재  
//	fMontageSectionPlaytime = EndTime - StartTime;
//	UE_LOG(LogTemp, Warning, TEXT("Attack %d  Montage playTime is : % f"), index, fMontageSectionPlaytime);
//
//}
