// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingThornSpawner.h" 
#include "MovingThorn.h" 
#include "Trap.h" 

AMovingThornSpawner::AMovingThornSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMovingThornSpawner::BeginPlay()
{
	Super::BeginPlay(); 
	
	if (HasAuthority()) 
	{ 
		SpawnMovingThorn(); 
	} 
}

void AMovingThornSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AMovingThornSpawner::SpawnMovingThorn()
{ 
	FTransform t = GetActorTransform(); 
	t.SetScale3D(FVector(3.0f, 3.0f, 2.0f)); 
	GetWorld()->SpawnActor<AMovingThorn>(MovingThornFactory, t); 
	t.SetLocation(t.GetLocation() + SpawnOffset);
	auto P2 = GetWorld()->SpawnActor<AMovingThorn>(MovingThornFactory, t); 
	P2->Idx = 1; 

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, [&]()
	{
		SpawnMovingThorn();
	}, FMath::RandRange(2.0f, 5.0f), false);
}
