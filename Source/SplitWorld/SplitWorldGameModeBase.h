// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SplitWorldGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SPLITWORLD_API ASplitWorldGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASplitWorldGameModeBase();
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override; 

	UPROPERTY() 
	TArray<class AController*> Players;
	
};
