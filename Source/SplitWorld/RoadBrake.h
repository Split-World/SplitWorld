// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "RoadBrake.generated.h"

/**
 * 
 */
UCLASS()
class SPLITWORLD_API ARoadBrake : public ATrap
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
    virtual void Execute(AActor* Interactor) override;
	
};


