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
		SetActorRotation(FQuat::Slerp(FRotator(0).Quaternion(), FRotator(0, 450.0f, 0).Quaternion(), GM->DoorGauge / 5.0f));
		Door->SetActorLocation(Door->GetActorLocation() + FVector(0, 0, -100.0f) * DeltaTime); 
	} 
}

