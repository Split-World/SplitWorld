// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "GameFramework/Actor.h"
#include "Thornbush.generated.h"

UCLASS()
class SPLITWORLD_API AThornbush : public ATrap 
{
	GENERATED_BODY()
	
public:	
	AThornbush(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Execute() override;
	
};
