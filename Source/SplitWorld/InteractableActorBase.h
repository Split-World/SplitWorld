// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractableActorBase.generated.h"

UCLASS()
class SPLITWORLD_API AInteractableActorBase : public AActor, public IInteractable  
{
	GENERATED_BODY()
	
public: 
	AInteractableActorBase(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Interaction_Implementation() override;

	UPROPERTY(EditAnywhere, Replicated)
	int Idx;
	
};
