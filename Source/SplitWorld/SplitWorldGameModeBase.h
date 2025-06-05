// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SplitWorldGameModeBase.generated.h"

UENUM()
enum class EMapPart : uint8
{ 
	Part1,
	Part2,
	Part2_5,
	Part3,
	Part4 
};

UCLASS()
class SPLITWORLD_API ASplitWorldGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASplitWorldGameModeBase();
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override; 
	virtual void Tick(float DeltaTime) override; 

	void ChangeMapPart(EMapPart Part); 
	
	UPROPERTY() 
	TArray<class AController*> Players;

	int bPlayer_Interactions[3];

	FVector2D PlayerScreenLocation[2]; 

	EMapPart CurPart; 

};
