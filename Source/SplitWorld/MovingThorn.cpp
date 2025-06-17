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
} 

void AMovingThorn::BeginPlay()
{
	Super::BeginPlay();

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
	if (!Idx && Player)
	{
		Player->Die(); 
	}
}

void AMovingThorn::Server_SetMesh_Implementation(int _Idx)
{ 
	Idx = _Idx; 
	Multi_SetMesh(_Idx); 
}

void AMovingThorn::Multi_SetMesh_Implementation(int _Idx)
{ 
	MeshComp->SetStaticMesh(Meshes[_Idx]);
	MeshComp->SetMaterial(0, Materials[_Idx]); 
}
