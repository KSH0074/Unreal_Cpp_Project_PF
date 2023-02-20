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
	
	//마우스커서를 중앙으로 하는 편의성 함수
	UFUNCTION()
	void CenterMouseCursor();

	//게임시작 직후에는 ViewPort사이즈가 0,0 임  따라서 0.2초후에 옮겨주는 것으로 하기 위한 타이머
	FTimerHandle MousePointInit;
	
	class APlayerController* varPlayerController;

	UPROPERTY()
	class AAIController* ai;

};
