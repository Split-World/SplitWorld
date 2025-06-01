// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingThornSpawner.generated.h"

UCLASS()
class SPLITWORLD_API AMovingThornSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingThornSpawner(); 
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override; 

	void SpawnMovingThorn(); 

private: 
	UPROPERTY(EditAnywhere) 
	TSubclassOf<class AMovingThorn> MovingThornFactory; 

	UPROPERTY(EditAnywhere) 
	FVector SpawnOffset = FVector(0, 50000.0f, 0); 

	FTimerHandle SpawnTimerHandle; 

};
