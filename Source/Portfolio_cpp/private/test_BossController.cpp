// Fill out your copyright notice in the Description page of Project Settings.


#include "test_BossController.h"
#include "BossMonster.h"
void Atest_BossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	me = Cast<ABossMonster>(InPawn);
}
Atest_BossController::Atest_BossController()
{
}
void Atest_BossController::BeginPlay()
{
	Super::BeginPlay();

}