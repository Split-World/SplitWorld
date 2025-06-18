// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h" 
#include "SplitPlayer.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h" 

ALaser::ALaser()
{ 
	PrimaryActorTick.bCanEverTick = true; 
	
	LaserComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserComp"));
	LaserComp->SetupAttachment(BoxComp);
	LaserComp->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	
	SetReplicates(true);
	bAlwaysRelevant = true; 
}

void ALaser::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
} 

void ALaser::Execute()
{ 
	if (Player) 
	{
		Player->Die();
	} 
}

void ALaser::Disable_Implementation()
{
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetVisibility(false);
	LaserComp->Deactivate(); 
}
