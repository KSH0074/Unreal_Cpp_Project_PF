// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.h"
#include <queue>
#include "MainPlayer.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FCommandOutDelegate,int32,Damage);

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

	//AttackBox
	UPROPERTY(VisibleAnywhere, Category = HitBox)
		class UBoxComponent* PlayerFootBox;
	UPROPERTY(VisibleAnywhere, Category = HitBox)
		class UBoxComponent* PlayerFootBox2;
	UPROPERTY(VisibleAnywhere, Category = HitBox)
		class UBoxComponent* PlayerPunchBox;
	//Temporary AttackBox
	UPROPERTY()
		class UBoxComponent* PlayerTempBox{};
	


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

	UPROPERTY(BlueprintReadOnly)
		FString commandUI{};
	UPROPERTY(BlueprintReadOnly)
		FString SkillUI{};
	UFUNCTION()
	void CommandTimeOut();

	UPROPERTY()
	bool isTimeEnd;

	UPROPERTY()
	FTimerHandle commandTimerHandle;
	
	UFUNCTION()
	void TableRead(FString Command, int32& damage);

	UFUNCTION()
	void TimeOver();
	
	UPROPERTY()
		class UPFGameInstance* thisGameInstance;

	FCommandOutDelegate UseSkill;

	UFUNCTION()
	void JangPoong(int32 Damage);
	
	int32 mPlayerPower{};

	UFUNCTION(BlueprintCallable)
	void ThrowFireball();

	UFUNCTION()
		void HurricaneKick(int32 Damage);
	UFUNCTION()
		void Dodge(int32 Damage);
	UFUNCTION()
		void BackDash(int32 Damage);
	UFUNCTION()
		void NormalAttack(int32 Damage);
	UFUNCTION()
		void FlyingKick(int32 Damage);

	//대미지 받는 함수
	UFUNCTION()
		void OnDamageProcess(int32 damage);


	UPROPERTY(EditDefaultsOnly, Category = FireBallFactory)
		TSubclassOf<class AFireBall> FireBall;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FireBallFactory)
		class UArrowComponent* firePosition2;

	//Player's HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP = 100;

	//Player Stamina 추후 구현 
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 mStamina = 100;*/
	UPROPERTY()
		class UMainPlayerAnim* Playeranim{};

	UFUNCTION()
	void AllowInput(bool bInputAllow = true);

	bool bInput = true;



	UFUNCTION()
	void FootBoxBeginOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void FootBoxEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	bool bHit= false;

	UPROPERTY()
	class AEnemy* mHittedEnemy{};

	UFUNCTION()	
	void AttackZoneControl(class UBoxComponent* box, bool tof);

	UFUNCTION()
		void MovementModeChange(UCharacterMovementComponent* const MovementComp, EMovementMode moveMode);

	UPROPERTY(BlueprintReadOnly)
	bool isDead = false;
	
};
