// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "InteractableActorBase.h"
#include "Crack.generated.h"

UCLASS()
class SPLITWORLD_API ACrack : public AInteractableActorBase 
{
	GENERATED_BODY()
	
public:	
	ACrack(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction_Implementation() override; 

private: 
	UPROPERTY(EditAnywhere) 
	class UBoxComponent* BoxComp; 

	UPROPERTY()
	class ASplitWorldGameModeBase* GM; 

};
