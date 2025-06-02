// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorHandle.h" 
#include "SplitWorldGameModeBase.h" 
#include "Net/UnrealNetwork.h"

ADoorHandle::ADoorHandle()
{ 
	PrimaryActorTick.bCanEverTick = true;

}

void ADoorHandle::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode()); 
}

void ADoorHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ContinuousInputCount = FMath::Lerp(ContinuousInputCount, 0.0f, 6 * DeltaTime); 
	GM->InputGauge[Idx] = ContinuousInputCount; 
}

void ADoorHandle::Interaction_Implementation()
{
	if (GM->bPlayer_Interactions & ~(1 << Idx)) GM->bPlayer_Interactions |= (1 << Idx);

	if (GM->bPlayer_Interactions != 3)
	{
		return; 
	}

	if (ContinuousInputCount < MaxContinuousInputCount)
	{
		ContinuousInputCount = FMath::Min(ContinuousInputCount + 1, MaxContinuousInputCount); 
	} 
}

