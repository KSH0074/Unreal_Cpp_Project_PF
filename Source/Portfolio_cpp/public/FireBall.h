// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireBall.generated.h"

UCLASS()
class PORTFOLIO_CPP_API AFireBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireBall();	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//�߻�ü ������Ʈ , projectile 
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* BulletProjectileComp;

private:
	//Collision
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent* collisionComp;
	
	//Static Mesh
	UPROPERTY(VisibleAnywhere, Category = BodyMesh)
	class UStaticMeshComponent* staticMeshComp;

	void Die();
public:
	UFUNCTION()
	void OnFireBallOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//�߻��� �÷��̾ ����ϴ� ����
	// �� �κ��� instigator�� ��ü ����
	//UPROPERTY()
	//class AMainPlayer* master;

	float fireballDamage;
};
