// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorHandleDevice.generated.h"

UCLASS()
class SPLITWORLD_API ADoorHandleDevice : public AActor
{
	GENERATED_BODY()
	
public: 
	ADoorHandleDevice(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

private: 
	UPROPERTY()
	class ASplitWorldGameModeBase* GM;

	UPROPERTY(EditAnywhere)
	int Idx; 

	UPROPERTY(EditAnywhere) 
	TArray<class ADoorHandle*> DoorHandles;
	
};
