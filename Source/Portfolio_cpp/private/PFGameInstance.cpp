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
		//테스트 코드 
		//tableSize = DtTable.Object->GetTableData().Num();
	}
}

FCommandTable* UPFGameInstance::GetTableData(FString column)
{
	FCommandTable* temp;
	temp = commandTable->FindRow<FCommandTable>(*FString(column), TEXT(""));
	if (temp != nullptr)
	{
		TextOut = temp->Text;
		return temp;
	}
	return nullptr;
}


