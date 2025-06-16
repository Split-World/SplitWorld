// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPLITWORLD_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetIMG(int _Idx); 
	void ShowInteraction();
	void HideInteraction();
	void ChangeIMG(bool bChangeKey); 
	void ActiveKey();
	void DeactiveKey(); 
	
	UPROPERTY(meta=(BindWidget))
	class UImage* IMG_Interaction;
	UPROPERTY(meta=(BindWidget))
	class UImage* IMG_Key;
	
	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* Approach;
	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* FarAway; 
	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* InteractionToKey;
	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* KeyToInteraction; 
	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* ShowKey; 
	UPROPERTY(meta=(BindWidgetAnim), Transient) 
	class UWidgetAnimation* HideKey; 
	
private: 
	UPROPERTY(EditAnywhere)
	TArray<class UMaterial*> Interactions;
	UPROPERTY(EditAnywhere) 
	TArray<class UMaterial*> Keys; 
	
	int Idx;
	
};
