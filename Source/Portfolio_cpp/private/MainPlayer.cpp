// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PFGameInstance.h"//데이터 테이블관련 헤더 
#include "CommandDataTable.h"//Struct 

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
	TableReadTest(1);
	if (bMouseDown)
	{
		MainCharacterMoveInput();
	}
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//이동관련
	PlayerInputComponent->BindAction("Move", IE_Pressed, this, &AMainPlayer::MouseButtonDown);
	PlayerInputComponent->BindAction("Move", IE_Released, this, &AMainPlayer::MouseButtonRelease);
	//커맨드 관련 
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AMainPlayer::InputCommand);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AMainPlayer::InputCommand);
	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &AMainPlayer::InputCommand);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AMainPlayer::InputCommand);
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
	UE_LOG(LogTemp, Warning,  TEXT("Left : % d"), mCommand);
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

void AMainPlayer::InputCommand(FKey inputKey)
{
	switch (inputKey.GetFName().GetPlainNameString()[0])//GetPlainNameString => 이름부분만 추출 FString 으로 변환
	{
	case 'D':
		InputRight();
		break;
	case 'A':
		InputLeft();
		break;
	case 'S':
		InputDown();
		break;
	case 'W':
		InputUp();
		break;
	default:
		break;
	}
}

void AMainPlayer::OutputCommand() 
{
	FString a{}; //커맨드 Queue 를 FString 형태로 받는 변수 
	while (!commandQueue.empty())
	{
		a.AppendInt(static_cast<int32>(commandQueue.front()));
		commandQueue.pop();
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *a); //받은 커맨드 

	FString b{};
	b = "41";
	if (a.Equals(b)) //기술표와 대조하는 부분 csv파일로 간단하게 이름/기술명(추후 함수이름)으로 만든다음 델리게이트 bindUFunctino으로 이름 넘겨서 함수 호출시키면 되지 않을까?
	{
		UE_LOG(LogTemp, Warning, TEXT("oshida"));
	}
	
}

void AMainPlayer::CommandTimeOut()
{

		GetWorld()->GetTimerManager().SetTimer(commandTimerHandle,this,&AMainPlayer::OutputCommand,0.5,false);//OutputCommand가 대신 비우는 함수로
	
	
}



/*
행으로 찾았는데 이걸 커맨드로 어떻게 만들지
지금 인풋은 41 으로 들어오는데 
맵을 하나 또 준비해서 41:1 로 묶어야하나 


애초에 맵으로 41 : 함수 
*/
void AMainPlayer::TableReadTest(int32 colmn)
{
	UPFGameInstance* thisGameInstance = Cast<UPFGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (nullptr == thisGameInstance) return; // 게임인스턴스가 설정하지 않았을 경우 등.. 

	FCommandTable* CurrentStatData{};
	int32 Level{};
	FString LoadedCommand{};
	CurrentStatData = 	thisGameInstance->GetABCharacterData(colmn);
	if (CurrentStatData)
	{
		Level = colmn;
		LoadedCommand = CurrentStatData->Command;//찾은 커맨드 
		UE_LOG(LogTemp, Warning, TEXT("Level %d data exist."), Level);//레벨(행)
		UE_LOG(LogTemp, Warning, TEXT("Command %s data exist."), *LoadedCommand);
	}
	else
	{
		// 데이터 테이블에 없는 레벨일 때
		UE_LOG(LogTemp, Warning, TEXT("Level %d data doesn't exist."), Level);
	}
}


