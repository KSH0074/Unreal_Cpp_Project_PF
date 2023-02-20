// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Portfolio_cppGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class PORTFOLIO_CPP_API APortfolio_cppGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	APortfolio_cppGameModeBase();

 	virtual void BeginPlay() override;

	FVector2D out_ViewportSize ;
	
};
