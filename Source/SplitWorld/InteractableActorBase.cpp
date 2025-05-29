// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorBase.h"

#include "Net/UnrealNetwork.h"

AInteractableActorBase::AInteractableActorBase()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableActorBase, Idx); 
}

void AInteractableActorBase::Interaction_Implementation()
{
	IInteractable::Interaction_Implementation();
	
}

