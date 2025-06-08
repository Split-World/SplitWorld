// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingFloor.h"
#include "SplitWorldGameModeBase.h" 
#include "Net/UnrealNetwork.h" 

AMovingFloor::AMovingFloor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("Objects"));
	Mesh->SetIsReplicated(true);

	SetReplicates(true); 
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
}

void AMovingFloor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	} 
}

void AMovingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GM && GM->bPlayer_Interactions[2] == 3 && MoveDistance < 0.0f)
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * 3500.0f * DeltaTime); 
		for (auto handle : Handles)
		{
			handle->SetActorLocation(handle->GetActorLocation() + GetActorForwardVector() * 3500.0f * DeltaTime); 
		}

		MoveDistance -= 3500.0f * DeltaTime; 
	}
}

void AMovingFloor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMovingFloor, MoveDistance); 
}

