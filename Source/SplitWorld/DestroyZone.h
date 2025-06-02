// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "GameFramework/Actor.h"
#include "DestroyZone.generated.h"

UCLASS()
class SPLITWORLD_API ADestroyZone : public ATrap 
{
	GENERATED_BODY()
	
public:	
	ADestroyZone();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Execute() override;
	
};
