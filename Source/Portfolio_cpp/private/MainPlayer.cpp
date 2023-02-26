// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90.0f), FRotator(0, -90.0f, 0));
	}
	//Spring Arm 설정 
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(-45.0f, 0.0f, 135.0f));
	springArmComp->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	springArmComp->TargetArmLength = 300;
	springArmComp->bInheritYaw = false;

	//카메라 설정 
	cameraComp = CreateDefaultSubobject<UCameraComponent>("MainPlayerCamera");
	cameraComp->SetupAttachment(springArmComp);

	//캐릭터 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;

	this->bUseControllerRotationYaw = false;

	
	
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	mainPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Move", IE_Pressed, this, &AMainPlayer::MainCharacterMoveInput);
	//PlayerInputComponent->BindAction("Move", IE_Released, this, &AMainPlayer::MainCharacterMoveInput);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AMainPlayer::MainCharacterMoveInput);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AMainPlayer::MainCharacterMoveInput);
	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &AMainPlayer::MainCharacterMoveInput);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AMainPlayer::MainCharacterMoveInput);
}

void AMainPlayer::MainCharacterMoveInput()
{
	//if 떼지 않았을 경우 true => tick 
	UE_LOG(LogTemp, Warning, TEXT("MouseInput Recive"));
	FHitResult hitResult;
	mainPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, hitResult);
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *hitResult.Location.ToString());
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(mainPlayerController,hitResult.Location);

}

