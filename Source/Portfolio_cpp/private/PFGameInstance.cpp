// Fill out your copyright notice in the Description page of Project Settings.


#include "PFGameInstance.h"
#include "CommandDataTable.h"

//게임컴파일할때 로드됩니다.
UPFGameInstance::UPFGameInstance()
{
	FString CharacterDataPath = TEXT("DataTable'/Game/Blueprints/CommandData.CommandData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DtTable(*CharacterDataPath);

	if (DtTable.Succeeded())
	{
		commandTable = DtTable.Object;
		UE_LOG(LogTemp, Warning, TEXT("data Table LoadComplete"));
	}
}

FCommandTable* UPFGameInstance::GetABCharacterData(int32 column)
{
	return commandTable->FindRow<FCommandTable>(*FString::FromInt(column), TEXT(""));
}
