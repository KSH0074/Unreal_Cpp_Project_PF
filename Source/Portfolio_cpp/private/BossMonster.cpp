// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonster.h"

ABossMonster::ABossMonster()
{

}

void ABossMonster::BeginPlay()
{
	//Super::BeginPlay(); �θ�Ŭ���� ȣ�� ����
	Super::Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Boss %s"), *this->GetName());
}

void ABossMonster::Tick(float DeltaTime)
{
}
