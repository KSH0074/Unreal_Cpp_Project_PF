// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBall.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
// Sets default values
AFireBall::AFireBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Craeate collision
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));

	//Attach collision(Block All)
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	
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

}

// Called when the game starts or when spawned
void AFireBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

