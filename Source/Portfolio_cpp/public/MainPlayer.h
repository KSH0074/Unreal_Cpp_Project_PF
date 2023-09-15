// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <queue>
#include "MainPlayer.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FCommandOutDelegate,int,Damage);

UENUM(BlueprintType)
enum class COMMAND : uint8
{
	None,
	Right = 1,
	Left,
	Up,
	Down,
};
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
	
	//HitBox
	UPROPERTY(VisibleAnywhere, Category = HitBox)
		class UBoxComponent* PlayerHitBox;

	void MainCharacterMoveInput();

	UPROPERTY()
	class APlayerController* mainPlayerController;

	bool bMouseDown = false;
	
	void MouseButtonDown();

	void MouseButtonRelease();

	void InputRight();
	void InputLeft();
	void InputUp();
	void InputDown();
	void InputCommand(FKey inputKey);

	void OutputCommand();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Command)
	COMMAND mCommand = COMMAND::None;

	
	std::queue<COMMAND> commandQueue;

	UFUNCTION()
	void CommandTimeOut();

	UPROPERTY()
	bool isTimeEnd;

	UPROPERTY()
	FTimerHandle commandTimerHandle;
	
	UFUNCTION()
	void TableRead(FString Command, int& damage);

	UFUNCTION()
	void TimeOver();
	
	UPROPERTY()
		class UPFGameInstance* thisGameInstance;

	FCommandOutDelegate UseSkill;

	UFUNCTION()
		void JangPoong(int Damage);
	int mfireBalldamage{};

	UFUNCTION(BlueprintCallable)
	void ThrowFireball();

	UFUNCTION()
		void Hold(int Damage);
	UFUNCTION()
		void Dodge(int Damage);
	UFUNCTION()
		void BackDash(int Damage);

	//대미지 받는 함수
	UFUNCTION()
		void OnDamageProcess(int damage);


	UPROPERTY(EditDefaultsOnly, Category = FireBallFactory)
		TSubclassOf<class AFireBall> FireBall;
	
	UPROPERTY(BlueprintReadWrite, Category = FireBallFactory)
		class UArrowComponent* firePosition;

	//Player's HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP = 10;

	UPROPERTY()
		class UMainPlayerAnim* Playeranim{};

	UFUNCTION()
	void AllowInput(bool bInputAllow = true);

	bool bInput = true;

	
};
