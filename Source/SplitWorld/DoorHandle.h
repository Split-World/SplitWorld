// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "InteractableActorBase.h"
#include "DoorHandle.generated.h"

UCLASS()
class SPLITWORLD_API ADoorHandle : public AInteractableActorBase 
{
	GENERATED_BODY()
	
public: 
	ADoorHandle(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction_Implementation() override; 

private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	class USceneComponent* Player_PointComp; 
	
	UPROPERTY()
	class ASplitWorldGameModeBase* GM; 

	FTimerHandle DoorInputTimerHandle; 
	 
}; 
