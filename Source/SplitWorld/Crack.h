// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "Interactable.h" 
#include "Crack.generated.h"

UCLASS()
class SPLITWORLD_API ACrack : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ACrack(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction_Implementation() override; 

};
