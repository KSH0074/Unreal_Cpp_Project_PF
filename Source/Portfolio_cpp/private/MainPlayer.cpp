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
	UE_LOG(LogTemp, Warning, TEXT("%d"), commandQueue.size());
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bMouseDown)
	{
		MainCharacterMoveInput();
	}
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Move", IE_Pressed, this, &AMainPlayer::MainCharacterMoveInput);
	PlayerInputComponent->BindAction("Move", IE_Pressed, this, &AMainPlayer::MouseButtonDown);
	PlayerInputComponent->BindAction("Move", IE_Released, this, &AMainPlayer::MouseButtonRelease);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AMainPlayer::InputRight);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AMainPlayer::InputLeft);
	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &AMainPlayer::InputUp);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AMainPlayer::InputDown);
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMainPlayer::OutputCommand);
}

void AMainPlayer::MainCharacterMoveInput()
{
	//if 떼지 않았을 경우 true => tick 
	if (bMouseDown)
	{
		UE_LOG(LogTemp, Warning, TEXT("MouseInput Recive"));
		FHitResult hitResult;
		mainPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, hitResult);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *hitResult.Location.ToString());
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(mainPlayerController, hitResult.Location);

		
	}
}

void AMainPlayer::MouseButtonDown()
{
	bMouseDown = true;
}

void AMainPlayer::MouseButtonRelease()
{
	bMouseDown = false;
}

void AMainPlayer::InputRight()
{
	mCommand = COMMAND::Right;
	UE_LOG(LogTemp, Warning, TEXT("Right : %d"),mCommand);
	commandQueue.push(mCommand);
	CommandTimeOut();
}

void AMainPlayer::InputLeft()
{
	mCommand = COMMAND::Left;
	UE_LOG(LogTemp, Warning, TEXT("Left : %d"), mCommand);
	commandQueue.push(mCommand);
	CommandTimeOut();
}

void AMainPlayer::InputUp()
{
	mCommand = COMMAND::Up;
	UE_LOG(LogTemp, Warning, TEXT("Up : %d"), mCommand);
	commandQueue.push(mCommand);
	CommandTimeOut();
}

void AMainPlayer::InputDown()
{
	mCommand = COMMAND::Down;
	UE_LOG(LogTemp, Warning, TEXT("Down : %d"), mCommand);
	commandQueue.push(mCommand);
	CommandTimeOut();
}

void AMainPlayer::OutputCommand()
{
	FString a{};
	while (!commandQueue.empty())
	{
		a.AppendInt(static_cast<int32>(commandQueue.front()));
		commandQueue.pop();
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *a);

	FString b{};
	b = "41";
	if (a.Equals(b))
	{
		UE_LOG(LogTemp, Warning, TEXT("oshida"));
	}
	
}

void AMainPlayer::CommandTimeOut()
{

		GetWorld()->GetTimerManager().SetTimer(commandTimerHandle,this,&AMainPlayer::OutputCommand,0.5,false);
	
	
}


/*
Fstring으로 Switch 문을 만들려고 하면 사용할 것 
// 문자열과 매칭되는 정수형 데이터를 저장하는 TMap
TMap<FString, int32> StringToIntMap;
StringToIntMap.Add(TEXT("apple"), 1);
StringToIntMap.Add(TEXT("banana"), 2);
StringToIntMap.Add(TEXT("cherry"), 3);

// Switch문에 사용할 문자열 변수
FString Fruit = TEXT("banana");

// 문자열과 매칭되는 정수형 데이터를 TMap에서 가져옴
int32 IntValue = -1;
if (StringToIntMap.Contains(Fruit))
{
    IntValue = *StringToIntMap.Find(Fruit);
}

// Switch문에서 매핑된 정수형 데이터를 사용
switch (IntValue)
{
case 1:
    UE_LOG(LogTemp, Warning, TEXT("This is an apple."));
    break;
case 2:
    UE_LOG(LogTemp, Warning, TEXT("This is a banana."));
    break;
case 3:
    UE_LOG(LogTemp, Warning, TEXT("This is a cherry."));
    break;
default:
    UE_LOG(LogTemp, Warning, TEXT("This is not a valid fruit."));
    break;
}
*/