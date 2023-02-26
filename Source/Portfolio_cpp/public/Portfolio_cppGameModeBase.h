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

	
	//마우스커서를 중앙으로 하는 편의성 함수
	UFUNCTION()
		void CenterMouseCursor();

	//게임시작 직후에는 ViewPort사이즈가 0,0 임  따라서 0.2초후에 옮겨주는 것으로 하기 위한 타이머
	FTimerHandle MousePointInit;
	class APlayerController* varPlayerController;
};
