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
