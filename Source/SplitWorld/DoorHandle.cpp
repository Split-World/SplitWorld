// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorHandle.h" 
#include "SplitWorldGameModeBase.h"
#include "Components/BoxComponent.h"

ADoorHandle::ADoorHandle()
{ 
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxComp);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	SetReplicates(true); 
	bAlwaysRelevant = true; 
}

void ADoorHandle::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	} 
}

void ADoorHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ADoorHandle::Interaction_Implementation()
{
	if (GM->bPlayer_Interactions[0] == 3) 
	{ 
		GM->DoorInput |= (1 << Idx); 
		GetWorldTimerManager().ClearTimer(DoorInputTimerHandle); 
		GetWorldTimerManager().SetTimer(DoorInputTimerHandle, [&]()
		{
			GM->DoorInput &= ~(1 << Idx); 
		}, 0.5f, false); 
	}
	
	if (!(GM->bPlayer_Interactions[0] & (1 << Idx)))
	{
		GM->bPlayer_Interactions[0] |= (1 << Idx);
	} 
} 
