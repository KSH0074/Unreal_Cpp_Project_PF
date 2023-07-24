// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_CPP_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	//여기에 애니메이션 재생을 조정하는 변수를 Enemy로부터 옮긴다.
	//true, false 값의 변화는 애니메이션 노티파이를 이용하여 애니메이션이 끝나는 지점에 설치한다. 
	//Enemy가 EnemyAnim을 include 하여 몽타주 재생에 관여할 수 있도록한다.
};
