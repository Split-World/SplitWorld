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
	auto P1 = GetWorld()->SpawnActor<AMovingThorn>(MovingThornFactory, t); 
	t.SetLocation(t.GetLocation() + SpawnOffset); 
	auto P2 = GetWorld()->SpawnActor<AMovingThorn>(MovingThornFactory, t); 
	P2->Idx = 1; 
	P2->Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly); 

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, [&]()
	{
		SpawnMovingThorn();
	}, FMath::RandRange(1.5f, 2.5f), false);
}
