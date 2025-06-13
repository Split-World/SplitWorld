// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitWorldGameModeBase.h" 

ASplitWorldGameModeBase::ASplitWorldGameModeBase()
{ 
	PrimaryActorTick.bCanEverTick = true; 

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Script/Engine.Blueprint'/Game/JS_Folder/BluePrints/BP_SplitPlayer.BP_SplitPlayer_C'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	} 
}

AActor* ASplitWorldGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	Players.Add(Player);
	
	return Super::ChoosePlayerStart_Implementation(Player); 
}

void ASplitWorldGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 

	if (CurPart == EMapPart::PartDoor)
	{ 
		RotateDoorHandle(DeltaTime); 
	} 

	if (CurPart == EMapPart::PartCrack)
	{
		CrackInput(DeltaTime); 
	}

	switch (CurPart)
	{
	case EMapPart::Part1:
		if (bPlayer_Interactions[0] == 3)
		{
			ChangeMapPart(EMapPart::PartDoor); 
		} 
		break; 
	case EMapPart::Part3_5: 
		if (bPlayer_Interactions[2] == 3) 
		{
			ChangeMapPart(EMapPart::Part4); 
		}
		break; 
	case EMapPart::Part4: 
		if (bPlayer_Interactions[3] == 3)
		{
			ChangeMapPart(EMapPart::PartCrack);
		}
		break; 
	} 
}

void ASplitWorldGameModeBase::ChangeMapPart(EMapPart Part)
{
	CurPart = Part; 
	ChangePartDelegate.Broadcast();
}

void ASplitWorldGameModeBase::RotateDoorHandle(float DeltaTime)
{ 
	//if (DoorInput == 3) 
	if (DoorInput) 
	{ 
		DoorGauge += DeltaTime; 
		if (DoorGauge >= 10.0f)
		{ 
			DoorGauge = 10.0f; 
			ChangeMapPart(EMapPart::Part2); 
		}
	} 
}

void ASplitWorldGameModeBase::CrackInput(float DeltaTime)
{ 
	CrackGauge[0] = FMath::Max(0.0f, CrackGauge[0] + DeltaTime * (bPlayer_Interactions[3] & 1 ? 1 : -1)); 
	CrackGauge[1] = FMath::Max(0.0f, CrackGauge[1] + DeltaTime * (bPlayer_Interactions[3] & 2 ? 1 : -1)); 
}
