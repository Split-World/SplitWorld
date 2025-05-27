// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "Interactable.h"
#include "Door.generated.h"

UCLASS()
class SPLITWORLD_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public: 
	ADoor(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction_Implementation() override; 

};
