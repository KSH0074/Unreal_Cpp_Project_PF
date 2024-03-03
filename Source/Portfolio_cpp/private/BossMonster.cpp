// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include <Components/CapsuleComponent.h>
#include "MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h" 
#include "test_BossController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "test_BossAnim.h"
#include <Components/BoxComponent.h>

ABossMonster::ABossMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder < USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/BossMonster/Characters/Heroes/Grux/Meshes/Grux.Grux'"));

	
	UE_LOG(LogTemp, Warning, TEXT("BossMonster Initialize"));

	attackZoneComp->SetRelativeScale3D(FVector(2.0f, 2.0f, 3.5f));
	UE_LOG(LogTemp, Warning, TEXT("child %s"), *this->GetName());

	HP = 500;
	fMeleeAttackRange = 350.0f;
	mDamage = 10;

	
}

void ABossMonster::BeginPlay()
{
	//Super::BeginPlay(); //부모클래스 호출 막기 애니메이션컴포넌트와 컨트롤러가 다른 클래스이므로 캐스트 되지 않음 
	Super::Super::BeginPlay();
	Scene_Placed_PlayerPawn = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));
	anim = Cast<Utest_BossAnim>(GetMesh()->GetAnimInstance());
	mController = Cast<Atest_BossController>(GetController());
	//mController->blackboardComp->SetValueAsInt(FName::FName("HP"), HP); // BossMonster에만 있는 BB 키값 

}

void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("child call Distance: %f"), distance.Size());
	//아래 부분 blueprint로 구현
	if(distance.Size() >=750.0f && !(mController->getBlackBoardState("IsCharge")))
		mController->blackboardComp->SetValueAsBool(FName::FName("IsCharge"), true);
	
}

void ABossMonster::OnDamageProcess(int32 damage)
{
	Super::OnDamageProcess(damage);
	mController->blackboardComp->SetValueAsInt(FName::FName("HP"), HP);
}

void ABossMonster::Attack()
{
	FString sectionName{};
	if (bLeftAttack)
	{
		 sectionName ="Attack0";
	}
	else 
		 sectionName = "Attack1";

	anim->PlayAttackAnim(FName(*sectionName));//BP에서 구현된 함수가 실행됨 



	isMontagePlaying = true;
	GetController()->StopMovement();

	UE_LOG(LogTemp, Warning, TEXT("Boss's Attack! %s"), *sectionName);

	bLeftAttack = !bLeftAttack;
}
