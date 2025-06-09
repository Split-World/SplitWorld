// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyZone.h"

#include "SplitPlayer.h"

ADestroyZone::ADestroyZone()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADestroyZone::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADestroyZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestroyZone::Execute()
{
	if (Player)
	{
		Player->Die(); 
	}
}

