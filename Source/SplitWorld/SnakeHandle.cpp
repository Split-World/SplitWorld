// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHandle.h"

#include "Snake.h"
#include "SplitWorldGameModeBase.h"

ASnakeHandle::ASnakeHandle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("Objects"));

	SetReplicates(true); 
	bAlwaysRelevant = true; 
}

void ASnakeHandle::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	} 
}

void ASnakeHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeHandle::Interaction_Implementation()
{
	if (!bActive && !(GM->bPlayer_Interactions[1] & 1))
	{
		bActive = true;
		for (auto Snake : Snakes)
		{
			Snake->bActive = true; 
		} 
		
		GetWorldTimerManager().SetTimer(ActiveTimerHandle, [&]()
		{
			if (!(GM->bPlayer_Interactions[1] & 2)) 
			{
				bActive = false;
				for (auto Snake : Snakes)
				{
					Snake->bActive = false; 
				} 
			} 
		}, 1.0f, false); 
	}
}

