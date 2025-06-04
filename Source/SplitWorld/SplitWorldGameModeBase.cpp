// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitWorldGameModeBase.h" 

ASplitWorldGameModeBase::ASplitWorldGameModeBase()
{
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

	switch (CurPart)
	{
	case EMapPart::Part1:
		if (bPlayer_Interactions[0] == 3)
		{
			CurPart = EMapPart::Part2; 
		} 
		break;
	case EMapPart::Part2: 
		break; 
	case EMapPart::Part2_5: 
		break;
	case EMapPart::Part3: 
		if (bPlayer_Interactions[2] == 3)
		{
			CurPart = EMapPart::Part2; 
		}
		break; 
	} 
}
