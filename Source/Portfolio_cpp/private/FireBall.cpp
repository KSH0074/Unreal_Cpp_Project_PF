// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBall.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "Enemy.h"
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

	//InitialLifeSpan = 2.0f;// Distroy or LifeSpan 사용하여 수명을 정함 
	//다른 물체에 충돌하였을 때도 사라져야 하므로 Distroy 사용 
}

// Called when the game starts or when spawned
void AFireBall::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle DeathTimer;
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, 2.0f, false);
	//바인드
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
//충돌(겹침)이벤트 발생 시 호출
void AFireBall::OnFireBallOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* enemy = Cast<AEnemy>(OtherActor); // OtherActor => 충돌한 다른 물체 

	if (enemy != nullptr)
	{
		//적 체력 추가시 변형, 임시 데미지 500.0f
		enemy->OnDamageProcess(500.0f);
	}
	//자신 제거
	Destroy();
}
