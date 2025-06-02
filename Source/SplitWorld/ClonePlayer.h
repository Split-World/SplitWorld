// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ClonePlayer.generated.h"

UCLASS()
class SPLITWORLD_API AClonePlayer : public APawn
{
	GENERATED_BODY()

public: 
	AClonePlayer(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override; 

};
