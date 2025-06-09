// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActorBase.h"
#include "GameFramework/Actor.h"
#include "SnakeHandle.generated.h"

UCLASS()
class SPLITWORLD_API ASnakeHandle : public AInteractableActorBase
{
	GENERATED_BODY()
	
public: 
	ASnakeHandle(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction_Implementation() override; 

private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY()
	class ASplitWorldGameModeBase* GM;
	
	UPROPERTY(EditAnywhere)
	TArray<class ASnake*> Snakes;

	bool bActive;
	FTimerHandle ActiveTimerHandle; 
	
};
