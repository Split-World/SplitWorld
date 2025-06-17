// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorHandle.h" 
#include "SplitWorldGameModeBase.h"
#include "Components/BoxComponent.h"

ADoorHandle::ADoorHandle()
{ 
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxComp);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	Player_PointComp = CreateDefaultSubobject<USceneComponent>(TEXT("Player_PointComp"));
	Player_PointComp->SetupAttachment(RootComponent);
	Player_PointComp->SetRelativeLocation(FVector(30.0f, 0, 0)); 
	Player_PointComp->SetRelativeRotation(FRotator(0, 180.0f, 0)); 
	Player_PointComp->SetIsReplicated(true); 
	
	bReplicates = true;
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
}

void ADoorHandle::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	} 
}

void ADoorHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GM && GM->CurPart == EMapPart::Part2)
	{
		GM->Players[Idx]->GetPawn()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); 
	}
}

void ADoorHandle::Interaction_Implementation()
{
	Super::Interaction_Implementation();
	
	if (GM->bPlayer_Interactions[0] == 3) 
	{ 
		GM->DoorInput |= (1 << Idx);
		
		GetWorldTimerManager().ClearTimer(DoorInputTimerHandle); 
		GetWorldTimerManager().SetTimer(DoorInputTimerHandle, [&]()
		{
			GM->DoorInput &= ~(1 << Idx); 
		}, 0.1f, false); 
	}
	
	if (!(GM->bPlayer_Interactions[0] & (1 << Idx)))
	{
		GM->bPlayer_Interactions[0] |= (1 << Idx);
		GM->Players[Idx]->GetPawn()->AttachToComponent(Player_PointComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Multi_SetVisibility(); 
	} 
} 
