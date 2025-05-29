// Fill out your copyright notice in the Description page of Project Settings.


#include "ClonePlayer.h"

AClonePlayer::AClonePlayer()
{ 
	PrimaryActorTick.bCanEverTick = true;

	bAlwaysRelevant = true;
}

void AClonePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	JumpMaxCount = 3;
}

void AClonePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClonePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

