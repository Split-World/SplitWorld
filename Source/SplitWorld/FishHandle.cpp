// Fill out your copyright notice in the Description page of Project Settings.


#include "FishHandle.h"

#include "Fish.h"
#include "SplitWorldGameModeBase.h"

AFishHandle::AFishHandle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("Objects"));

	SetReplicates(true); 
	bAlwaysRelevant = true; 
}

void AFishHandle::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	} 
}

void AFishHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFishHandle::Interaction_Implementation()
{
	if (!bLaunched && !(GM->bPlayer_Interactions[1] & 1) && !(GM->bPlayer_Interactions[1] & 2))
	{
		bLaunched = true;
		GM->bPlayer_Interactions[1] |= 1; 

		Fishes[0]->Launch(); 
		GetWorldTimerManager().SetTimer(FishTimerManager, [&]()
		{
			Fishes[1]->Launch(); 
			GetWorldTimerManager().SetTimer(FishTimerManager, [&]()
			{
				Fishes[2]->Launch();
				GetWorldTimerManager().SetTimer(FishTimerManager, [&]()
				{
					bLaunched = false;
					GM->bPlayer_Interactions[1] &= ~1; 
				}, 0.75f, false); 
			}, 0.25f, false); 
		}, 0.25f, false); 
	}
}

