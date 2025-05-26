// Fill out your copyright notice in the Description page of Project Settings.


#include "TempPlayer.h"
#include "Net/UnrealNetwork.h" 

ATempPlayer::ATempPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATempPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATempPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATempPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATempPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{ 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ATempPlayer, Clone);
}
