// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"
#include <Components/CapsuleComponent.h>
#include "MainPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h" 

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

	attackZoneComp->SetRelativeScale3D(FVector(2.0f, 2.0f, 3.5f));
	UE_LOG(LogTemp, Warning, TEXT("child %s"), *this->GetName());

	
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay(); //부모클래스 호출 막기 애니메이션컴포넌트와 컨트롤러가 다른 클래스이므로 캐스트 되지 않음 
	Super::Super::BeginPlay();
	Scene_Placed_PlayerPawn = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));

}

void ABossMonster::Tick(float DeltaTime)	
{
	
}

void ABossMonster::OnDamageProcess(int32 damage)
{
}

void ABossMonster::DeathState()
{
}

void ABossMonster::Attack()
{
}

void ABossMonster::attackZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABossMonster::attackZoneEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
