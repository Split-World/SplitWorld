// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPLITWORLD_API UWaitingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public: 
	virtual void NativeConstruct() override; 
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; 

	UFUNCTION() 
	void ConnectionBreak(); 
	UFUNCTION() 
	void GameStart(); 

	UPROPERTY(meta=(BindWidget)) 
	class UButton* Btn_GameStart; 
	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_ConnectionBreak; 
	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_Player1; 
	UPROPERTY(meta=(BindWidget))
	class UBorder* BD_Player2;
	UPROPERTY(meta=(BindWidget)) 

	UPROPERTY()
	class USplitWorldGameInstance* GI; 
	UPROPERTY() 
	class AGameStateBase* GS; 

};
