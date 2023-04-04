// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PFGameInstance.generated.h"

/**
 * 
 */

struct FCommandTable;
class UDataTable;

UCLASS()
class PORTFOLIO_CPP_API UPFGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPFGameInstance();
	FCommandTable* GetABCharacterData(FString column);
	
	//TestCode
	//int32 tableSize = 0;
	UPROPERTY()
	FString TextOut;
private:
	UPROPERTY()
	UDataTable* commandTable;
};
