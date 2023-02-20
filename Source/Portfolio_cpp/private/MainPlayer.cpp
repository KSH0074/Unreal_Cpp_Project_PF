// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Camera/CameraComponent.h>
#include "Kismet/GameplayStatics.h"
#include <AIController.h>
// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90.0f), FRotator(0,-90.0f,0));
	}
	//Spring Arm 설정 
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(-45.0f ,0.0f, 135.0f));
	springArmComp->SetRelativeRotation(FRotator(-60.0f ,0.0f,  0.0f));
	springArmComp->TargetArmLength = 300;
	springArmComp->bInheritYaw = false;

	//카메라 설정 
	cameraComp = CreateDefaultSubobject<UCameraComponent>("MainPlayerCamera");
	cameraComp->SetupAttachment(springArmComp);
	
	//캐릭터 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	this->bUseControllerRotationYaw = false;
	ai = Cast<AAIController>(this->GetController());
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	//이 시점에서는 viewport생성이 안되어 있음 0.2초 뒤 생성 함, 컴퓨터 사양에 따라 작동 안될 위험이 있음. 추후 뷰포트 생성이후 바꾸는 형식으로 해야 될 지도?
	GetWorld()->GetTimerManager().SetTimer(MousePointInit, this, &AMainPlayer::CenterMouseCursor,0.2f);
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
}

void AMainPlayer::MainCharacterMoveInput()
{
	UE_LOG(LogTemp, Warning, TEXT("MouseInput Recive"));
	//LineTrace를 통해서 카메라 부터 선을 쏴서 부딪힌 지점에 이동하도록 할 것
	//시작지점 
	FVector startPosition = cameraComp->GetComponentLocation();
	//종료지점? 
	FVector endPosition = startPosition + cameraComp->GetForwardVector() * 50000;

	//충돌정보 담은 변수
	FHitResult hitResult;
	
	//충돌옵션
	FCollisionQueryParams params;
	//플레이어는 무시 
	params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByObjectType(hitResult, startPosition, endPosition, ECC_EngineTraceChannel1, params);
	
	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"),*hitResult.ImpactPoint.ToString());
		//if (ai != nullptr)
		//{
		//	ai->MoveToLocation(hitResult.ImpactPoint);
		//}
		
	}
}

void AMainPlayer::CenterMouseCursor()
{
	// Get the viewport size
	int32 ViewportSizeX, ViewportSizeY;

	varPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	varPlayerController->bShowMouseCursor = true;
	varPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	// Set the mouse position to the center of the viewport
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(ViewportSizeX / 2, ViewportSizeY / 2);
}