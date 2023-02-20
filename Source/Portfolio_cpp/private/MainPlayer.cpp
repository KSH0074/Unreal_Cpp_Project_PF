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
	//Spring Arm ���� 
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(-45.0f ,0.0f, 135.0f));
	springArmComp->SetRelativeRotation(FRotator(-60.0f ,0.0f,  0.0f));
	springArmComp->TargetArmLength = 300;
	springArmComp->bInheritYaw = false;

	//ī�޶� ���� 
	cameraComp = CreateDefaultSubobject<UCameraComponent>("MainPlayerCamera");
	cameraComp->SetupAttachment(springArmComp);
	
	//ĳ���� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	this->bUseControllerRotationYaw = false;
	ai = Cast<AAIController>(this->GetController());
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	//�� ���������� viewport������ �ȵǾ� ���� 0.2�� �� ���� ��, ��ǻ�� ��翡 ���� �۵� �ȵ� ������ ����. ���� ����Ʈ �������� �ٲٴ� �������� �ؾ� �� ����?
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
	//LineTrace�� ���ؼ� ī�޶� ���� ���� ���� �ε��� ������ �̵��ϵ��� �� ��
	//�������� 
	FVector startPosition = cameraComp->GetComponentLocation();
	//��������? 
	FVector endPosition = startPosition + cameraComp->GetForwardVector() * 50000;

	//�浹���� ���� ����
	FHitResult hitResult;
	
	//�浹�ɼ�
	FCollisionQueryParams params;
	//�÷��̾�� ���� 
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