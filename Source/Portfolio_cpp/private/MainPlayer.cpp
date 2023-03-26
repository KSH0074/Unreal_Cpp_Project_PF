// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PFGameInstance.h"//데이터 테이블관련 헤더 
#include "CommandDataTable.h"//Struct 
#include <Components/ArrowComponent.h>
#include "FireBall.h"
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

	//firePosition 
	firePosition2 = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition2"));
	firePosition2->SetRelativeLocation(FVector(80.0f, 0.0f, 40.0f));
	firePosition2->bHiddenInGame = false;
	firePosition2->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	mainPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
//	UE_LOG(LogTemp, Warning, TEXT("%d"), commandQueue.size());
	thisGameInstance = Cast<UPFGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//TableRead("41");
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
	UE_LOG(LogTemp, Warning,  TEXT("Right : % d"), mCommand);
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

	TableRead(a);//스킬 바인딩까지 
	if (UseSkill.IsBound()) //
	{
		UseSkill.Execute();
	}
	
}

void AMainPlayer::CommandTimeOut()
{
		GetWorld()->GetTimerManager().SetTimer(commandTimerHandle,this,&AMainPlayer::TimeOver,0.5,false);//OutputCommand가 대신 비우는 함수로
}



/*
행 이름 숫자 말고 문자로 할 수 있는거 처음 알았다 ...
*/
void AMainPlayer::TableRead(FString InputCommand)// 인수 FString 으로 변경하여 위의 OutputCommand 에 적ㅇ요할 수 있도록 하기 
{
	UseSkill.Unbind();//이미 바인드 되어있는거 해제 
	if (nullptr == thisGameInstance) return; // 게임인스턴스가 설정하지 않았을 경우 등.. 

	FCommandTable* temp; //커맨드 테이블(행)을 저장할 임시변수 
	temp = thisGameInstance->GetABCharacterData(InputCommand); // PFGameInstance에 구현한 GetABCharacterData사용 InputCommand와 이름이 같은 행을 찾아 반환 
	if (temp != nullptr) // 찾은 경우 
	{
		UE_LOG(LogTemp, Warning, TEXT("OutputCommand : %s"), *thisGameInstance->TextOut); //출력로그에 출력  
		UseSkill.BindUFunction(this, *thisGameInstance->TextOut); //Blueprint에서 바인딩하려고 했으나 일단은 C++상으로 구현 해당 커맨드와 함수를 바인딩 
	}
	else //못 찾은 경우 
	{
		return;
	}
}
void AMainPlayer::TimeOver()
{
	while (!commandQueue.empty())
	{
		commandQueue.pop();
	}
	UE_LOG(LogTemp, Warning, TEXT("TimeOver : Input CommandRemoved"));
}


void AMainPlayer::JangPoong()
{
	UE_LOG(LogTemp, Warning, TEXT("use Skill jangpoong"));
	GetWorld()->SpawnActor<AFireBall>(FireBall, firePosition2->GetComponentTransform());
	//여기에 현재 마우스 포인트 바라보게하는 코드 추가 
}

void AMainPlayer::Hold()
{
	UE_LOG(LogTemp, Warning, TEXT("use Skill Hold"));
}

void AMainPlayer::Dodge()
{
	UE_LOG(LogTemp, Warning, TEXT("use Skill Dodge"));
}

void AMainPlayer::BackDash()
{
	UE_LOG(LogTemp, Warning, TEXT("use Skill BackDash"));
}


