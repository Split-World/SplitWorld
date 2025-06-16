// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrackWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPLITWORLD_API UCrackWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetIMG(int Idx);
	
	UPROPERTY(meta=(BindWidget)) 
	class UImage* IMG_Key;
	UPROPERTY(meta=(BindWidget)) 
	class UImage* IMG_CircularProgressBar; 
	
private: 
	UPROPERTY(EditAnywhere)
	TArray<class UMaterial*> Keys;
	UPROPERTY(EditAnywhere) 
	TArray<class UMaterial*> ProgressBars; 
	
}; 
