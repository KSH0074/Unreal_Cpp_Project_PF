// Fill out your copyright notice in the Description page of Project Settings.


#include "PFGameInstance.h"
#include "CommandDataTable.h"

//�����������Ҷ� �ε�˴ϴ�.
UPFGameInstance::UPFGameInstance()
{
	FString CharacterDataPath = TEXT("DataTable'/Game/Blueprints/CommandData.CommandData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DtTable(*CharacterDataPath);

	if (DtTable.Succeeded())
	{
		commandTable = DtTable.Object;
		UE_LOG(LogTemp, Warning, TEXT("data Table LoadComplete"));
		//�׽�Ʈ �ڵ� 
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


