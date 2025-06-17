// Fill out your copyright notice in the Description page of Project Settings.


#include "CrackWidget.h" 
#include "Components/Image.h"

void UCrackWidget::SetIMG(int Idx)
{
	IMG_Key->SetBrushFromMaterial(Keys[Idx]);
	IMG_CircularProgressBar->SetBrushFromMaterial(ProgressBars[Idx]); 

	IMG_Key->SetVisibility(ESlateVisibility::Hidden);  
	IMG_CircularProgressBar->SetVisibility(ESlateVisibility::Hidden); 
}
