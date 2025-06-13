// Fill out your copyright notice in the Description page of Project Settings.


#include "Crack.h" 
#include "Components/BoxComponent.h" 
#include "SplitWorldGameModeBase.h" 

ACrack::ACrack()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp->SetCollisionProfileName(TEXT("Crack"));
	BoxComp->SetIsReplicated(true); 

	SetReplicates(true);
	bAlwaysRelevant = true; 
}

void ACrack::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	} 
}

void ACrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrack::Interaction_Implementation()
{ 
	Super::Interaction_Implementation();
	
	if (!(GM->bPlayer_Interactions[3] & (1 << Idx)))
	{
		GM->bPlayer_Interactions[3] |= (1 << Idx);
	}
}

