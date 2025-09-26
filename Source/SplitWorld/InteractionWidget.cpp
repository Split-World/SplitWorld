// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"

#include "Components/Image.h"

void UInteractionWidget::SetIMG(int _Idx)
{ 
	Idx = _Idx;
	
	IMG_Interaction->SetBrushFromMaterial(Interactions[Idx]); 
	IMG_Key->SetBrushFromMaterial(Keys[Idx]);

	IMG_Interaction->SetVisibility(ESlateVisibility::Hidden);  
	IMG_Key->SetVisibility(ESlateVisibility::Hidden);  
}

void UInteractionWidget::ShowInteraction()
{
	PlayAnimation(Approach); 
}

void UInteractionWidget::HideInteraction()
{
	PlayAnimation(FarAway); 
}

void UInteractionWidget::ChangeIMG(bool bChangeKey)
{
	PlayAnimation(bChangeKey ? InteractionToKey : KeyToInteraction); 
}

void UInteractionWidget::ActiveKey()
{
	PlayAnimation(ShowKey); 
}

void UInteractionWidget::DeactiveKey()
{
	PlayAnimation(HideKey); 
}
