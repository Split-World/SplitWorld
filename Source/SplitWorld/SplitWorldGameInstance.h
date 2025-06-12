// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SplitWorldGameInstance.generated.h"

UCLASS()
class SPLITWORLD_API USplitWorldGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override; 
	void CreateSession(int32 playerCount);

	IOnlineSessionPtr sessionInterface;

	
};
