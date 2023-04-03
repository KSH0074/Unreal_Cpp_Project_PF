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
	//�ٸ� ��ü�� �浹�Ͽ��� ���� ������� �ϹǷ� Distroy ��� 
}

// Called when the game starts or when spawned
void AFireBall::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle DeathTimer;
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, 2.0f, false);
	//���ε�
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AFireBall::OnFireBallOverlap);

}

// Called every frame
void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AFireBall::Die()
{
	Destroy();
}
//�浹(��ħ)�̺�Ʈ �߻� �� ȣ��, ������ ���ݰ� ���õ� MainPlayer�� �ۼ��ϴ°� ���� ������ 
void AFireBall::OnFireBallOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OtherActor => �浹�� �ٸ� ��ü�� Enemy�� �� ��ȯ �õ� 
	AEnemy* enemy = Cast<AEnemy>(OtherActor);  
	//�� ��ȯ���� == Enemy�� ���� ��� 
	if (enemy != nullptr)
	{
		//�� ü�� �߰��� ����, �ӽ� ������ 500.0f
		//DataTable���� ���� �޾ƿ� �����ϵ��� �ؾ��� 
		enemy->OnDamageProcess(Damage);
		UE_LOG(LogTemp, Warning, TEXT("Damage test:%f"),Damage);
	}
	//�ڽ� ����
	Destroy();
}
