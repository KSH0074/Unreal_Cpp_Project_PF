// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBall.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "Enemy.h"
#include "MainPlayer.h"
// Sets default values
AFireBall::AFireBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Craeate collision
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	collisionComp->SetGenerateOverlapEvents(true);
	collisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	collisionComp->SetCollisionObjectType(ECC_GameTraceChannel4);

	
	//Size 
	collisionComp->SetSphereRadius(10);
	//Attach Root
	RootComponent = collisionComp;

	staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyShapeStaticMesh"));
	staticMeshComp->SetupAttachment(collisionComp);
	staticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	staticMeshComp->SetRelativeScale3D(FVector(0.20));

	BulletProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	BulletProjectileComp->SetUpdatedComponent(collisionComp);

	BulletProjectileComp->InitialSpeed = 1500.0f;
	BulletProjectileComp->MaxSpeed = 2000.0f;
	BulletProjectileComp->bShouldBounce = false;


	//InitialLifeSpan = 2.0f;// Distroy or LifeSpan ����Ͽ� ������ ���� 
	//�ٸ� ��ü�� �浹�Ͽ��� ���� ������� �ϹǷ� Destroy ��� 
}

// Called when the game starts or when spawned
void AFireBall::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(DeathTimer,this,&AFireBall::Die ,2.0f, false);
	//���ε�
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFireBall::OnFireBallOverlap);

}


void AFireBall::Die()
{
	Destroy();
}
//�浹(��ħ)�̺�Ʈ �߻� �� ȣ�� FireBall �� �浹���� ��� �۵��ϴ� �Լ� 
void AFireBall::OnFireBallOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap!!"));
	// OtherActor => �浹�� �ٸ� ��ü�� Enemy�� �� ��ȯ �õ� 
	AEnemy* enemy = Cast<AEnemy>(OtherActor);  
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherComp->GetFName().ToString()); Enemy�� CollisionCylinder�� Overlap
	//�� ��ȯ���� == Enemy�� ���� ��� 
	if (enemy != nullptr)
	{
		
		//DataTable���� ���� �޾ƿ� �����ϵ��� �ؾ��� 
		enemy->OnDamageProcess(fireballDamage);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetInstigator()->GetFName().ToString()));
		
		UE_LOG(LogTemp, Warning, TEXT("Damage test:%d"), fireballDamage);
	}

	//�ڽ� ����
	Destroy();
}
