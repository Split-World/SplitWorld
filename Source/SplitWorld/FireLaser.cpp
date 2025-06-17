// Fill out your copyright notice in the Description page of Project Settings.


#include "FireLaser.h" 
#include "SplitPlayer.h" 
#include "Components/BoxComponent.h" 
#include "NiagaraComponent.h" 

AFireLaser::AFireLaser()
{
 	PrimaryActorTick.bCanEverTick = true;

	LaserComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserComp")); 
	LaserComp->SetupAttachment(BoxComp); 
	LaserComp->SetRelativeRotation(FRotator(-90.0f, 90.0f, 0.0f)); 
	LaserComp->SetRelativeScale3D(FVector(1.0f, 0.5f, 0.5f)); 

	SetReplicates(true);
	bAlwaysRelevant = true; 
}

void AFireLaser::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(false); 
	
	LaserComp->Deactivate(); 
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

	LaserComp->Activate(); 

	FTimerHandle handle; 
	GetWorldTimerManager().SetTimer(handle, [&]()
	{ 
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
		MeshComp->SetVisibility(false); 
	}, 0.5f, false); 
} 
