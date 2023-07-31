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
#include <Components/CapsuleComponent.h>
#include <Components/BoxComponent.h>
#include "FireBall.h"
#include <Kismet/KismetMathLibrary.h>
// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/ImportedAnimationAndCharacter/Player/Mesh/PlayerSkeletal.PlayerSkeletal'"));
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

	bUseControllerRotationYaw = false;

	//firePosition 
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition2"));
	firePosition->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	firePosition->bHiddenInGame = false;
	firePosition->SetupAttachment(RootComponent);

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel2);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);//Enenmy
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block); //Floor

	//hitBox 설정
	PlayerHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	PlayerHitBox->SetRelativeScale3D(FVector(1.0f, 1.0f, 3.25f));
	PlayerHitBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //Overlap만 이용하므로
	PlayerHitBox->SetCollisionObjectType(ECC_GameTraceChannel6);//PlayerHitBox

	PlayerHitBox->SetCollisionResponseToAllChannels(ECR_Ignore);//모든 콜리전 무시
	PlayerHitBox->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);//Enenmy에만 overlap
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
		//UE_LOG(LogTemp, Warning, TEXT("MouseInput Recive"));
		FHitResult hitResult;
		//ECC_VIsibility 트레이스 채널대신 MoveAble채널로 함 
		bool isMoveAble{};
		isMoveAble = mainPlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel5, false, hitResult);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *hitResult.Location.ToString());

		if(isMoveAble)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(mainPlayerController, hitResult.Location);
		}
			
		
		
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
	//커맨드 Queue 를 FString 형태로 받는 변수
	FString a{};  
	while (!commandQueue.empty())
	{
		a.AppendInt(static_cast<int32>(commandQueue.front()));
		commandQueue.pop();
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *a); //받은 커맨드 
	int tempDmg =0;
	
	TableRead(a,tempDmg);

	if (UseSkill.IsBound()) //
	{
		FHitResult hitResult;
		mainPlayerController->GetHitResultUnderCursor(ECC_Visibility, false, hitResult);
		
		FRotator turnPlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), hitResult.Location);
		SetActorRotation(FRotator(0.0f, turnPlayer.Yaw, 0.0f));
		
		UseSkill.Execute(tempDmg);
	}
	
}

//커맨드 0.5초 내 출력하지 않으면 취소됨 
void AMainPlayer::CommandTimeOut()
{
		GetWorld()->GetTimerManager().SetTimer(commandTimerHandle,this,&AMainPlayer::TimeOver,0.5,false);
}



/*
행 이름 숫자 말고 문자로 
*/

// 인수 FString 으로 변경하여 위의 OutputCommand 에 적용할 수 있도록 하기 
void AMainPlayer::TableRead(FString InputCommand,int& damage)
{
	UseSkill.Unbind();//이미 바인드 되어있는거 해제 
	if (thisGameInstance == nullptr) return; // 게임인스턴스가 설정하지 않았을 경우 등.. 

	FCommandTable* temp; //커맨드 테이블(행)을 저장할 임시변수 
	temp = thisGameInstance->GetABCharacterData(InputCommand); // PFGameInstance에 구현한 GetABCharacterData사용 InputCommand와 이름이 같은 행을 찾아 반환 
	
	if (temp != nullptr) // 찾은 경우 
	{
		FString skillDMG = temp->SkillDamage;
		UE_LOG(LogTemp, Warning, TEXT("OutputCommand : %s"), *thisGameInstance->TextOut); //출력로그에 출력  
		UseSkill.BindUFunction(this, *thisGameInstance->TextOut); //해당 커맨드와 함수를 바인딩 
		damage = FCString::Atoi(*skillDMG);
	}
	else //못 찾은 경우 
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill not found, faild TableReading"));
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


void AMainPlayer::JangPoong(int Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("use Skill jangpoong"));

	FActorSpawnParameters SpawnPrams;
	SpawnPrams.bNoFail = true;
	AFireBall* FireBallInstance = GetWorld()->SpawnActor<AFireBall>(FireBall, firePosition->GetComponentTransform());
	//생성위치가 겹칠때 생기는 문제를 방지하기 위해서 
	if( FireBallInstance != nullptr)
	{
		FireBallInstance->fireballDamage = Damage;
		AMainPlayer* selfPointer = this;
		FireBallInstance->SetInstigator(selfPointer);
	}

}

void AMainPlayer::Hold(int Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("use Skill Hold"));
}

void AMainPlayer::Dodge(int Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("use Skill Dodge"));
}

void AMainPlayer::BackDash(int Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("use Skill BackDash"));
}



