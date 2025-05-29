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

	if (InputGauge[0] + InputGauge[1] > 6.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Rotate Door Handle"));
	}
}
