// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"


DECLARE_LOG_CATEGORY_EXTERN(Player, Log, All);

#define PRINT_CALLINFO() UE_LOG(Player, Warning, TEXT("%s",(FStirng(__FUNCTION__)+TEXT("(")+FString::FromInt(__LINE__)+TEXT(")"))));