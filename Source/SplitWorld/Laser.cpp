// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h" 
#include "SplitPlayer.h"
#include "Components/BoxComponent.h"

ALaser::ALaser()
{ 
	PrimaryActorTick.bCanEverTick = true; 

	SetReplicates(true);
	bAlwaysRelevant = true; 
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

void ALaser::Disable_Implementation()
{
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(false);  
}
