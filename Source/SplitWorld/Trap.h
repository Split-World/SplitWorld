// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class SPLITWORLD_API ATrap : public AActor
{
	GENERATED_BODY()
	
public: 
	ATrap(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() 
	virtual void Execute() = 0; 

protected: 
	UPROPERTY() 
	class UBoxComponent* BoxComp; 

};
