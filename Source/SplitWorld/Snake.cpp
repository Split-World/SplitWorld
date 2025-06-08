// Fill out your copyright notice in the Description page of Project Settings.


#include "Snake.h"

#include "Net/UnrealNetwork.h"

ASnake::ASnake()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("Snake")); 

	SetReplicates(true); 
	bAlwaysRelevant = true; 
}

void ASnake::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnake::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASnake, bActive); 
} 
