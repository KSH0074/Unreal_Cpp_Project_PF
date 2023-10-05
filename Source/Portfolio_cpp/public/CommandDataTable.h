// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h" 
#include "CommandDataTable.generated.h"

USTRUCT(BlueprintType)
struct FCommandTable : public FTableRowBase
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Command;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Text;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString SkillDamage;

	//������ �� �ʱ�ȭ
	FCommandTable() :Command(""), Text(""), SkillDamage("")
	{

	};
};
UCLASS()
class PORTFOLIO_CPP_API ACommandDataTable : public AActor
{ 
	GENERATED_BODY()

};
