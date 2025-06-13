// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SplitWorldGameModeBase.generated.h" 

DECLARE_MULTICAST_DELEGATE(FChangePart) 

UENUM(BlueprintType)
enum class EMapPart : uint8
{ 
	Part1, 
	PartDoor, 
	Part2, 
	Part2_5, 
	Part3, 
	Part3_5, 
	Part4, 
	PartCrack 
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
	void RotateDoorHandle(float DeltaTime); 
	void CrackInput(float DeltaTime); 
	
	UPROPERTY() 
	TArray<class AController*> Players; 

	int bPlayer_Interactions[4];
	int bViewChanger[8]; 

	EMapPart CurPart = EMapPart::Part1; 

	FChangePart ChangePartDelegate; 

	int DoorInput; 
	float DoorGauge; 
	
	float CrackGauge[2]; 

};
