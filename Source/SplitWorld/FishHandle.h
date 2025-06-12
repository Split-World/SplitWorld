// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActorBase.h"
#include "GameFramework/Actor.h"
#include "FishHandle.generated.h"

UCLASS()
class SPLITWORLD_API AFishHandle : public AInteractableActorBase 
{
	GENERATED_BODY()
	
public:	
	AFishHandle(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interaction_Implementation() override; 

private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY()
	class ASplitWorldGameModeBase* GM;

	bool bLaunched; 
	FTimerHandle FishTimerHandle;

	UPROPERTY(EditAnywhere)
	TArray<class AFish*> Fishes;

	UPROPERTY(EditAnywhere)
	TArray<class AFireLaser*> FireLasers;

	UPROPERTY(EditAnywhere)
	class ALaser* NormalLaser; 
	
};
