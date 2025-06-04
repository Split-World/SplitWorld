// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "SplitWorldGameModeBase.h"

ADoor::ADoor()
{
 	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("Objects"));
	Mesh->SetIsReplicated(true);

	SetReplicates(true); 
	bAlwaysRelevant = true; 
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	} 
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	
	if (!bActive && GM && GM->bPlayer_Interactions[0] == 3)
	{
		bActive = true; 
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	} 
}
