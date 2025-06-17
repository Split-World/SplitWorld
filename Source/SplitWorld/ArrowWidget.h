// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArrowWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPLITWORLD_API UArrowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetIMG(); 
	void HideUI(); 
	void ShowUI(); 
	
	UPROPERTY(meta=(BindWidget)) 
	class UImage* IMG_Arrow;

	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* MoveArrow; 
	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* Hide;
	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* Show; 
	
};
