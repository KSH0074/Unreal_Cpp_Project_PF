// Copyright Epic Games, Inc. All Rights Reserved.


#include "Portfolio_cppGameModeBase.h"


APortfolio_cppGameModeBase::APortfolio_cppGameModeBase()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")")));
	ConstructorHelpers::FClassFinder<APawn> tempChar(TEXT("Blueprint'/Game/Blueprints/BP_MainPlayer.BP_MainPlayer_C'"));
	if (tempChar.Succeeded())
	{
		this->DefaultPawnClass = tempChar.Class;
	}
}
void APortfolio_cppGameModeBase::BeginPlay()
{

	Super::BeginPlay();


}