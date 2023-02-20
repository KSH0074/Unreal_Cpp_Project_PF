// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainPlayer.generated.h"

UCLASS()
class PORTFOLIO_CPP_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class  USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* cameraComp;
	
	UFUNCTION()
	void MainCharacterMoveInput();
	
	//���콺Ŀ���� �߾����� �ϴ� ���Ǽ� �Լ�
	UFUNCTION()
	void CenterMouseCursor();

	//���ӽ��� ���Ŀ��� ViewPort����� 0,0 ��  ���� 0.2���Ŀ� �Ű��ִ� ������ �ϱ� ���� Ÿ�̸�
	FTimerHandle MousePointInit;
	
	class APlayerController* varPlayerController;

	UPROPERTY()
	class AAIController* ai;

};
