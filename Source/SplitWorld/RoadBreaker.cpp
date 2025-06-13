// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadBreaker.h" 
#include "Engine/StaticMeshActor.h" 
#include "Net/UnrealNetwork.h"

ARoadBreaker::ARoadBreaker()
{ 
	PrimaryActorTick.bCanEverTick = true;

}

void ARoadBreaker::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoadBreaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ARoadBreaker::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoadBreaker, Roads); 
} 

void ARoadBreaker::Execute()
{ 
	for (auto Road : Roads)
	{
		auto Comp = Road->GetStaticMeshComponent(); 
		Comp->SetSimulatePhysics(true); 
	}
} 

