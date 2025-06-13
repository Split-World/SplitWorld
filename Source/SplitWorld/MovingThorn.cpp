// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingThorn.h"
#include "SplitPlayer.h" 
#include "Net/UnrealNetwork.h" 
#include "Components/BoxComponent.h" 

AMovingThorn::AMovingThorn()
{ 
	PrimaryActorTick.bCanEverTick = true; 
	
	bReplicates = true; 
	SetReplicateMovement(true);
	bAlwaysRelevant = true; 
} 

void AMovingThorn::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->SetStaticMesh(Meshes[Idx]);
	MeshComp->SetMaterial(0, Materials[Idx]); 
}

void AMovingThorn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Idx && BoxComp->GetCollisionProfileName() != TEXT("Objects")) 
	{
		BoxComp->SetCollisionProfileName(TEXT("Objects"));
	}
	
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime); 
}

void AMovingThorn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{ 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(AMovingThorn, Idx); 
}

void AMovingThorn::Execute()
{ 
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("MovingThron")); 
	if (!Idx && Player)
	{
		Player->Die(); 
	}
}

void AMovingThorn::OnRep_IdxChange()
{
	MeshComp->SetStaticMesh(Meshes[Idx]);
	MeshComp->SetMaterial(0, Materials[Idx]); 
}

