// Fill out your copyright notice in the Description page of Project Settings.


#include "Thornbush.h" 
#include "SplitPlayer.h"

AThornbush::AThornbush()
{ 
	PrimaryActorTick.bCanEverTick = true;

}

void AThornbush::BeginPlay()
{
	Super::BeginPlay();
	
}

void AThornbush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AThornbush::Execute()
{ 
	if (Player) 
	{
		Player->Die(); 
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Thornbush"));
	} 
}

