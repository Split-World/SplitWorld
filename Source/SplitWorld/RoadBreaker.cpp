// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadBreaker.h" 
#include "Engine/StaticMeshActor.h" 

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

void ARoadBreaker::Execute()
{
	for (auto Road : Roads)
	{ 

	}
}

