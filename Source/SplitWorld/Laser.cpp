// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h" 
#include "SplitPlayer.h"

ALaser::ALaser()
{ 
	PrimaryActorTick.bCanEverTick = true;

}

void ALaser::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaser::Execute()
{ 
	if (Player) 
	{
		Player->Die(); 
	} 
}
