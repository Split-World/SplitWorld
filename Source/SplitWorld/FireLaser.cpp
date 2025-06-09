// Fill out your copyright notice in the Description page of Project Settings.


#include "FireLaser.h" 
#include "SplitPlayer.h" 
#include "Components/BoxComponent.h" 

AFireLaser::AFireLaser()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void AFireLaser::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(false); 
}

void AFireLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireLaser::Execute()
{ 
	if (Player)
	{
		Player->Die();
	}
}

void AFireLaser::Fire()
{ 
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
	MeshComp->SetVisibility(true); 

	FTimerHandle handle; 
	GetWorldTimerManager().SetTimer(handle, [&]()
	{ 
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
		MeshComp->SetVisibility(false); 
	}, 0.5f, false); 
} 
