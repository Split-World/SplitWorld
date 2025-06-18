// Fill out your copyright notice in the Description page of Project Settings.


#include "CrackWidget.h" 
#include "Components/Image.h"

void UCrackWidget::SetIMG(int Idx)
{
	IMG_Key->SetBrushFromMaterial(Keys[Idx]);
	IMG_CircularProgressBar->SetBrushFromMaterial(ProgressBars[Idx]); 
	IMG_CircleFrame->SetBrushFromMaterial(Frames[Idx]); 

	HideCrackUI(); 
}

void UCrackWidget::ShowCrackUI()
{
	PlayAnimation(ShowProgressBar); 
}

void UCrackWidget::HideCrackUI()
{
	IMG_Key->SetVisibility(ESlateVisibility::Hidden);  
	IMG_CircularProgressBar->SetVisibility(ESlateVisibility::Hidden); 
	IMG_CircleFrame->SetVisibility(ESlateVisibility::Hidden); 
}
