// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingThorn.h"
#include "SplitPlayer.h" 
#include "Net/UnrealNetwork.h" 
#include "Components/BoxComponent.h" 

AMovingThorn::AMovingThorn()
{ 
	PrimaryActorTick.bCanEverTick = true; 
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh")); 
	Mesh->SetupAttachment(BoxComp); 
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	bReplicates = true; 
	SetReplicateMovement(true); 
} 

void AMovingThorn::BeginPlay()
{
	Super::BeginPlay();

	if (Idx)
	{
		BoxComp->SetCollisionProfileName(TEXT("MovingThorn")); 
	}
}

void AMovingThorn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime); 
}

void AMovingThorn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{ 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(AMovingThorn, Idx); 
}

void AMovingThorn::Execute()
{ 
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("MovingThron")); 
	if (!Idx && Player)
	{
		Player->Die();
	}
}

