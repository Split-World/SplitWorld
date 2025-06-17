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
	t.SetScale3D(FVector(4.0f, 4.0f, 1.0f)); 
	auto P1 = GetWorld()->SpawnActor<AMovingThorn>(MovingThornFactory, t); 
	P1->Server_SetMesh(0);
	t.SetLocation(t.GetLocation() + SpawnOffset);
	auto P2 = GetWorld()->SpawnActor<AMovingThorn>(MovingThornFactory, t); 
	P2->Server_SetMesh(1); 

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, [&]()
	{
		SpawnMovingThorn();
	}, FMath::RandRange(4.0f, 6.5f), false);
}
