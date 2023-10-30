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

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("AnimBlueprint'/Game/BossMonster/Characters/Heroes/Grux/Grux_AnimBlueprint.Grux_AnimBlueprint_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}

	ConstructorHelpers::FClassFinder <AAIController> tempAIClass(TEXT("Blueprint'/Game/BossMonster/Characters/Heroes/Grux/Boss_AI/BP_BossController.BP_BossController_C'"));
	if (tempAIClass.Succeeded())
	{
		AIControllerClass = tempAIClass.Class;
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Faild give Default AIclass to Boss"));
	}

	UE_LOG(LogTemp, Warning, TEXT("BossMonster Initialize"));

	attackZoneComp->SetRelativeScale3D(FVector(2.0f, 2.0f, 3.5f));
	UE_LOG(LogTemp, Warning, TEXT("child %s"), *this->GetName());

	HP = 500;
	//sfMeleeAttackRange = 350.0f;
	mDamage = 1;

	
}

void ABossMonster::BeginPlay()
{
	//Super::BeginPlay(); //�θ�Ŭ���� ȣ�� ���� �ִϸ��̼�������Ʈ�� ��Ʈ�ѷ��� �ٸ� Ŭ�����̹Ƿ� ĳ��Ʈ ���� ���� 
	Super::Super::BeginPlay();
	Scene_Placed_PlayerPawn = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));
	anim = Cast<Utest_BossAnim>(GetMesh()->GetAnimInstance());
	mController = Cast<Atest_BossController>(GetController());
	mController->blackboardComp->SetValueAsInt(FName::FName("HP"), HP); // BossMonster���� �ִ� BB Ű�� 

}

void ABossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("parent call Distance: %f"), distance.Size());
	//�Ʒ� �κ� blueprint�� ����
	if(distance.Size() >=500.0f && !(mController->getBlackBoardState("IsCharge")))
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

	anim->PlayAttackAnim(FName(*sectionName));//BP���� ������ �Լ��� ����� 



	isMontagePlaying = true;
	GetController()->StopMovement();

	UE_LOG(LogTemp, Warning, TEXT("Boss's Attack! %s"), *sectionName);

	bLeftAttack = !bLeftAttack;
}
