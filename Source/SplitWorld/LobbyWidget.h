// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h" 

UCLASS()
class SPLITWORLD_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public: 
	virtual void NativeConstruct() override; 

	UFUNCTION() 
	void Play(); 

	UPROPERTY(meta=(BindWidget)) 
	class UButton* Btn_Play; 

	UPROPERTY()
	class USplitWorldGameInstance* GI; 
	
};
