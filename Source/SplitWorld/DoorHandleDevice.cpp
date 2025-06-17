// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorHandleDevice.h" 
#include "SplitWorldGameModeBase.h" 
#include "Door.h" 

ADoorHandleDevice::ADoorHandleDevice()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetReplicates(true);
	bAlwaysRelevant = true;
}

void ADoorHandleDevice::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	}
}

void ADoorHandleDevice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) 
	{ 
		float Yaw = FMath::Lerp(0, 450.0f, GM->DoorGauge / 10.0f); 
		SetActorRotation(FRotator(0, Yaw, 0.0f)); 
	}  
} 
