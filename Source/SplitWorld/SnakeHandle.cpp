// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeHandle.h"

#include "Snake.h"
#include "SplitWorldGameModeBase.h"
#include "Components/BoxComponent.h"

ASnakeHandle::ASnakeHandle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxComp);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	bReplicates = true;
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
	Super::Interaction_Implementation();
	
	if (!bActive && !(GM->bPlayer_Interactions[1] & 4))
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
				for (auto Snake : Snakes)
				{
					Snake->bActive = false; 
				}
				
				GetWorldTimerManager().SetTimer(ActiveTimerHandle, [&]()
				{
					bActive = false;
					Multi_ExecuteFunction(EFunctionType::ActiveKey, false); 
				}, 1.0f, false); 
			}
			else
			{
				Multi_SetVisibility(); 
			}
		}, 1.0f, false); 
	}
}

