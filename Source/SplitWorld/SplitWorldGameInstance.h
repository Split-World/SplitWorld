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
	
	void CreateSession();
	void FindSession();
	void JoinSession(); 

private:
	UFUNCTION()
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful); 
	UFUNCTION()
	void OnFindSessionsComplete(bool bWasSuccessful); 
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result); 

	IOnlineSessionPtr SessionInterface; 
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	FString SessionName = "Default Room";
	FString HostName = "SplitPlayer";

	int SessionIndex;
	FString FindSessionName; 
	
};

