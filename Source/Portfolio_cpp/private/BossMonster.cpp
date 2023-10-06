// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"

ABossMonster::ABossMonster()
{

}

void ABossMonster::BeginPlay()
{
	//Super::BeginPlay(); 부모클래스 호출 막기
	Super::Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Boss %s"), *this->GetName());
}

void ABossMonster::Tick(float DeltaTime)
{
}
