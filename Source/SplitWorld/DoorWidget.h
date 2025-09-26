// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DoorWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPLITWORLD_API UDoorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetIMG(int Idx);
	void InteractionHandle();
	void HideUI();
	void ShowUI();

	UPROPERTY(meta=(BindWidget)) 
	class UImage* IMG_Key; 

	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* Interaction;
	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* Hide;
	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* Show; 

private: 
	UPROPERTY(EditAnywhere)
	TArray<class UMaterial*> Keys;
	
};
