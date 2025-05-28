// Fill out your copyright notice in the Description page of Project Settings.


#include "Raser.h" 
#include "SplitPlayer.h"

ARaser::ARaser()
{ 
	PrimaryActorTick.bCanEverTick = true;

}

void ARaser::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARaser::Execute()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Raser"));

	if (Player) 
	{
		Player->Die(); 
	} 
}
