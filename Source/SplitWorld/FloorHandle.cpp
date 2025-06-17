// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorHandle.h" 
#include "SplitWorldGameModeBase.h"
#include "Components/BoxComponent.h"

AFloorHandle::AFloorHandle()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxComp);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	bReplicates = true;
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
}

void AFloorHandle::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	} 
}

void AFloorHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorHandle::Interaction_Implementation()
{
	Super::Interaction_Implementation();
	
	if (!(GM->bPlayer_Interactions[2] & (1 << Idx)))
	{ 
		GM->bPlayer_Interactions[2] |= (1 << Idx);
		Multi_SetVisibility(); 
	} 
}
