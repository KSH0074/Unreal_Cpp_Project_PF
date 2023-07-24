// Copyright Epic Games, Inc. All Rights Reserved.


#include "Portfolio_cppGameModeBase.h"
#include "Kismet/GameplayStatics.h"

APortfolio_cppGameModeBase::APortfolio_cppGameModeBase()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(FString(__FUNCTION__) + TEXT("(test:") + FString::FromInt(__LINE__) + TEXT(")")));
	ConstructorHelpers::FClassFinder<APawn> tempChar(TEXT("Blueprint'/Game/Blueprints/BP_MainPlayer.BP_MainPlayer_C'"));
	if (tempChar.Succeeded())
	{
		this->DefaultPawnClass = tempChar.Class;
	}
}
void APortfolio_cppGameModeBase::BeginPlay()
{

	Super::BeginPlay();
	//�� ���������� viewport������ �ȵǾ� ���� 0.2�� �� ���� ��, ��ǻ�� ��翡 ���� �۵� �ȵ� ������ ����. ���� ����Ʈ �������� �ٲٴ� �������� �ؾ� ��
	GetWorld()->GetTimerManager().SetTimer(MousePointInit, this, &APortfolio_cppGameModeBase::CenterMouseCursor, 0.2f);


}
void APortfolio_cppGameModeBase::CenterMouseCursor()
{
	// Get the viewport size
	int32 ViewportSizeX, ViewportSizeY;

	varPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	varPlayerController->bShowMouseCursor = true;
	varPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	// Set the mouse position to the center of the viewport
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(ViewportSizeX / 2, ViewportSizeY / 2);

}