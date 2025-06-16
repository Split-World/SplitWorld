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

	bReplicates = true; 
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	StartLocation = GetActorLocation(); 
} 

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) 
	{
		if (GM->DoorInput)
		{
			SetActorLocation(FMath::Lerp(StartLocation, StartLocation + FVector(0, 0, -300.0f), GM->DoorGauge / 10.0f));
		} 
		else if (GM->CurPart == EMapPart::Part2 && GetActorLocation().Z > -800.0f) 
		{
			SetActorLocation(GetActorLocation() + FVector(0, 0, -1200.0f) * DeltaTime);
		}
	} 
}
