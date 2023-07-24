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
	//이 시점에서는 viewport생성이 안되어 있음 0.2초 뒤 생성 함, 컴퓨터 사양에 따라 작동 안될 위험이 있음. 추후 뷰포트 생성이후 바꾸는 형식으로 해야 함
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