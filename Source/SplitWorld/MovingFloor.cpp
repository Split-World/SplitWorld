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

	bReplicates = true;
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
}

void AMovingFloor::BeginPlay()
{
	Super::BeginPlay();
	
	if (Idx && HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	} 
}

void AMovingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Idx && GM && GM->bPlayer_Interactions[2] == 3 && MoveDistance > 0.0f)
	{
		FVector Dir = GetActorForwardVector() * -1500.0f * DeltaTime; 
		SetActorLocation(GetActorLocation() + Dir); 
		OtherFloor->SetActorLocation(OtherFloor->GetActorLocation() + Dir); 
		for (auto handle : Handles)
		{
			handle->SetActorLocation(handle->GetActorLocation() + Dir); 
		}

		MoveDistance -= 1500.0f * DeltaTime; 
	}
}

void AMovingFloor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMovingFloor, MoveDistance); 
}

