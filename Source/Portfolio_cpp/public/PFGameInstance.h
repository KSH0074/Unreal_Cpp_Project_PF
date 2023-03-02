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
	FCommandTable* GetABCharacterData(int32 column);

private:
	UPROPERTY()
	UDataTable* commandTable;
};
