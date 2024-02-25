// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "BossMonster.h"
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
#include "MainPlayerAnim.h"
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
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -70.0f), FRotator(0, -90.0f, 0));
	}
	//Spring Arm 설정 
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(-45.0f, 0.0f, 135.0f));
	springArmComp->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	springArmComp->TargetArmLength = 700;
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
	firePosition->bHiddenInGame = true;
	firePosition->SetupAttachment(RootComponent);

	//capsuleComponent 
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

	//attackBox
	// 
	//footBox
	PlayerFootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FootBox"));
	PlayerFootBox->SetupAttachment(GetMesh(), "LeftLeg");
	//PlayerFootBox->AttachTo(GetMesh(),"");
	PlayerFootBox->bHiddenInGame = false;
	PlayerFootBox->SetGenerateOverlapEvents(false);
	PlayerFootBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	PlayerFootBox->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	PlayerFootBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlayerFootBox->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);

	// Player가 왼발로 공격하는 것으로 모션 통일함, 따라서 필요 없어짐 
	////footBox2 
	//PlayerFootBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("FootBox2"));
	//PlayerFootBox2->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform,"RightUpLeg");
	//
	//PlayerFootBox2->bHiddenInGame = false;
	//PlayerFootBox2->SetGenerateOverlapEvents(false);
	//PlayerFootBox2->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	//PlayerFootBox2->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	//PlayerFootBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	//PlayerFootBox2->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);

	//punchBox
	PlayerPunchBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PunchBox"));
	PlayerPunchBox->SetupAttachment(GetMesh(), "LeftHand");
	PlayerPunchBox->bHiddenInGame = false;
	PlayerPunchBox->SetGenerateOverlapEvents(false);
	PlayerPunchBox->SetRelativeLocation(FVector(0.0f, -45.0f, 0.0f));
	PlayerPunchBox->SetRelativeScale3D(FVector(1.0f, 1.3f, 1.0f));

	PlayerPunchBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlayerPunchBox->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap);
	/*------------------------------------------------------------------------------------*/
	//공격판정존 델리게이트 연결 //이름 변경이 필요해보임 
	PlayerFootBox->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::FootBoxBeginOverlap);
	PlayerFootBox->OnComponentEndOverlap.AddDynamic(this, &AMainPlayer::FootBoxEndOverlap);

	PlayerPunchBox->OnComponentBeginOverlap.AddDynamic(this, &AMainPlayer::FootBoxBeginOverlap);
	PlayerPunchBox->OnComponentEndOverlap.AddDynamic(this, &AMainPlayer::FootBoxEndOverlap);

	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_NavWalking;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	mainPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	thisGameInstance = Cast<UPFGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Playeranim = Cast<UMainPlayerAnim>(GetMesh()->GetAnimInstance());

	PlayerHitBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));



}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	MainCharacterMoveInput();

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
	//if 떼지 않았을 경우 true => tick에서 계속 작동되어 드래그로도 움직이기 가능함, 이때 첫 우 클릭시 바닥에 이펙트를 주어서 어디로 이동하는 지 보여주는 코드를 추가할 예정
	if (bMouseDown && bInput)
	{
		//UE_LOG(LogTemp, Warning, TEXT("MouseInput Recive"));
		FHitResult hitResult;
		//ECC_VIsibility 트레이스 채널대신 MoveAble채널로 함 
		bool isMoveAble{};
		isMoveAble = mainPlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel5, false, hitResult);

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *hitResult.Location.ToString());

		if (isMoveAble)
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
	UE_LOG(Player, Warning, TEXT("Right : %d"), mCommand);
	commandQueue.push(mCommand);
	CommandTimeOut();
}

void AMainPlayer::InputLeft()
{
	mCommand = COMMAND::Left;
	UE_LOG(Player, Warning, TEXT("Left : %d"), mCommand);
	commandQueue.push(mCommand);
	CommandTimeOut();
}

void AMainPlayer::InputUp()
{
	mCommand = COMMAND::Up;
	UE_LOG(Player, Warning, TEXT("Up : %d"), mCommand);
	commandQueue.push(mCommand);
	CommandTimeOut();
}

void AMainPlayer::InputDown()
{
	mCommand = COMMAND::Down;
	UE_LOG(Player, Warning, TEXT("Down : %d"), mCommand);
	commandQueue.push(mCommand);
	CommandTimeOut();
}

void AMainPlayer::InputCommand(FKey inputKey)
{

	switch (inputKey.GetFName().GetPlainNameString()[0])//GetPlainNameString => 이름부분만 추출 FString 으로 변환
	{
	case 'D':
	{
		InputRight();
		commandUI += 'D';
		break;
	}

	case 'A':
	{
		InputLeft();
		commandUI += 'A';
		break;
	}
	case 'S':
		InputDown();
		commandUI += 'S';
		break;
	case 'W':
		InputUp();
		commandUI += 'W';
		break;
	default:
		break;
	}


}

void AMainPlayer::OutputCommand()
{
	if (bInput)
	{

		//커맨드 Queue 를 FString 형태로 받는 변수
		FString a{};
		while (!commandQueue.empty())
		{
			a.AppendInt(static_cast<int32>(commandQueue.front()));
			commandQueue.pop();
		}
		UE_LOG(Player, Warning, TEXT("%s"), *a); //받은 커맨드 
		int32 tempDmg = 0;

		TableRead(a, tempDmg);
		commandUI = "";
		//스킬사용 방향으로 플레이어 회전 
		if (UseSkill.IsBound())
		{
			FHitResult hitResult;
			mainPlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel5, false, hitResult);
			hitResult.Location.Z = GetActorLocation().Z;
			FRotator turnPlayer = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), hitResult.Location);
			SetActorRotation(FRotator(0.0f, turnPlayer.Yaw, 0.0f));
			SkillUI = *thisGameInstance->TextOut;
			UseSkill.Execute(tempDmg);
		}
	}

}

//커맨드 0.5초 내 출력하지 않으면 취소됨 
void AMainPlayer::CommandTimeOut()
{
	GetWorld()->GetTimerManager().SetTimer(commandTimerHandle, this, &AMainPlayer::TimeOver, 0.5, false);
}



/*
행 이름 숫자 말고 문자로
*/

// 인수 FString 으로 변경하여 위의 OutputCommand 에 적용할 수 있도록 하기 
void AMainPlayer::TableRead(FString InputCommand, int32& damage)
{
	UseSkill.Unbind();//이미 바인드 되어있는거 해제 
	if (thisGameInstance == nullptr) return; // 게임인스턴스가 설정하지 않았을 경우 등.. 

	FCommandTable* temp; //커맨드 테이블(행)을 저장할 임시변수 
	if (InputCommand.IsEmpty())
		temp = thisGameInstance->GetTableData(FString("NoCommand"));//NormalAttack 
	else
		temp = thisGameInstance->GetTableData(InputCommand); // PFGameInstance에 구현한 GetTableData사용 InputCommand와 이름이 같은 행을 찾아 반환 

	if (temp != nullptr) // 찾은 경우 
	{
		int32 skillDMG = temp->SkillDamage;
		UE_LOG(Player, Warning, TEXT("OutputCommand : %s"), *thisGameInstance->TextOut); //출력로그에 출력  
		UseSkill.BindUFunction(this, *thisGameInstance->TextOut); //해당 커맨드와 함수를 바인딩 
		damage = skillDMG;
		
	}
	else //못 찾은 경우 
	{
		UseSkill.BindUFunction(this, "NormalAttack");
		UE_LOG(Player, Warning, TEXT("Skill not found, faild TableReading"));
		SkillUI = "NormalAttack";
		return;
	}
}
void AMainPlayer::TimeOver()
{
	while (!commandQueue.empty())
	{
		commandQueue.pop();
	}
	UE_LOG(Player, Warning, TEXT("TimeOver : Input CommandRemoved"));
	commandUI = "";
}


void AMainPlayer::JangPoong(int32 Damage)
{
	UE_LOG(Player, Warning, TEXT("use Skill jangpoong"));

	Playeranim->PlaySkillMontage(1.5f, "FireBall");
	mPlayerPower = Damage;
}

void AMainPlayer::ThrowFireball()
{
	FActorSpawnParameters SpawnPrams;
	SpawnPrams.bNoFail = true;
	AFireBall* FireBallInstance = GetWorld()->SpawnActor<AFireBall>(FireBall, firePosition->GetComponentTransform());

	//생성위치가 겹칠때 생기는 문제를 방지하기 위해서 
	if (FireBallInstance != nullptr)
	{
		FireBallInstance->fireballDamage = mPlayerPower;
		AMainPlayer* selfPointer = this;
		FireBallInstance->SetInstigator(selfPointer);
	}
}

void AMainPlayer::HurricaneKick(int32 Damage)
{
	UE_LOG(Player, Warning, TEXT("use Skill HurricaneKick"));

	MovementModeChange(GetCharacterMovement(), EMovementMode::MOVE_Flying);

	//이 아래 네 줄의 코드를 함수화 할 수 있을것 
	PlayerTempBox = PlayerFootBox;
	AttackZoneControl(PlayerTempBox, true);

	Playeranim->PlaySkillMontage(1.5f, "HurricaneKick");
	mPlayerPower = Damage;
}

void AMainPlayer::Dodge(int32 Damage)
{
	UE_LOG(Player, Warning, TEXT("use Skill Dodge"));

	PlayerTempBox = PlayerHitBox;
	AttackZoneControl(PlayerTempBox, false);

	Playeranim->PlaySkillMontage(1.0f, "Dodge");
	mPlayerPower = Damage;
}

void AMainPlayer::BackDash(int32 Damage)
{
	UE_LOG(Player, Warning, TEXT("use Skill BackDash"));
}

void AMainPlayer::NormalAttack(int32 Damage)
{
	UE_LOG(Player, Warning, TEXT("use Skill NormalAttack"));

	PlayerTempBox = PlayerFootBox;
	AttackZoneControl(PlayerTempBox, true);

	Playeranim->PlaySkillMontage(0.8f, "NormalAttack");
	mPlayerPower = Damage;
}

void AMainPlayer::FlyingKick(int32 Damage)
{
	UE_LOG(Player, Warning, TEXT("use Skill FlyingKick"));

	//z축 움직임이 제한되어 일시적으로 Flying 하게 하는코드 
	MovementModeChange(GetCharacterMovement(), EMovementMode::MOVE_Flying);

	PlayerTempBox = PlayerFootBox;
	AttackZoneControl(PlayerTempBox, true);

	Playeranim->PlaySkillMontage(1.5, "FlyingKick");
	mPlayerPower = Damage;
}

void AMainPlayer::OnDamageProcess(int32 damage)
{
	//공중공격중 피격시 날아가는 문제 해결 
	MovementModeChange(GetCharacterMovement(), EMovementMode::MOVE_NavWalking);
	GetCharacterMovement()->StopMovementImmediately();
	//피격시 공격 취소됨, PlayDamageMontage 내에서 Play_Montage의 매개변수 중 bStopAllMontage = true,로 했음에도 몽타주가 취소되지 않았다. 
	Playeranim->StopAllMontages(1.0f);
	Playeranim->PlayDamageMontage();
	mainPlayerController->StopMovement();

	HP -= damage;
	UE_LOG(Player, Warning, TEXT("Player HP:%d"), HP);
	if (HP <= 0)
	{
		//사망 모션 이후 게임오버 창 띄우고 조작 안되도독 함, 
		//위 내용 구현 전까지는 게임을 끝내는 것으로 대체한다 
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); // 플레이어 컨트롤러 가져오기
		PlayerController->ConsoleCommand("Exit");
	}

}

void AMainPlayer::AllowInput(bool bInputAllow)
{
	bInput = bInputAllow;
	if (bInput)
	{
		UE_LOG(Player, Warning, TEXT("Player Controller Input true"));
	}
	else
	{
		UE_LOG(Player, Warning, TEXT("Player Controller Input false"));

	}
}

void AMainPlayer::FootBoxBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag("Enemy"))
	{
		mHittedEnemy = Cast<AEnemy>(OtherActor);

		UE_LOG(Player, Warning, TEXT("Attack Enemy : %s "), *(OtherActor->GetFName().ToString()));
	}
	else if (OtherActor->ActorHasTag("Boss"))
	{
		mHittedEnemy = Cast<ABossMonster>(OtherActor);

		UE_LOG(Player, Warning, TEXT("Attack Boss : %s "), *(OtherActor->GetFName().ToString()));
	}
	else
	{
		UE_LOG(Player, Warning, TEXT("Attack Enemy failed : %s "), *(OtherActor->GetFName().ToString()));
	}
	bHit = true;
}

void AMainPlayer::FootBoxEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)

{
	bHit = false;
}

void AMainPlayer::AttackZoneControl(UBoxComponent* box, bool tof)
{
	box->SetGenerateOverlapEvents(tof);
}

void AMainPlayer::MovementModeChange(UCharacterMovementComponent* const MovementComp, EMovementMode moveMode)
{
	MovementComp->SetMovementMode(moveMode);
}
