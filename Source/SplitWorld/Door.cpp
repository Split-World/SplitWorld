// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{ 
	PrimaryActorTick.bCanEverTick = true;

}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{ 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 
	
	DOREPLIFETIME(ADoor, Idx); 
}

void ADoor::Interaction_Implementation()
{ 

}

