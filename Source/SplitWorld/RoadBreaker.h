// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "GameFramework/Actor.h"
#include "RoadBreaker.generated.h"

UCLASS()
class SPLITWORLD_API ARoadBreaker : public ATrap
{
	GENERATED_BODY()
	
public: 
	ARoadBreaker(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;
	
	virtual void Execute() override; 
	
};
