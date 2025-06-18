// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorWidget.h" 
#include "Components/Image.h"

void UDoorWidget::SetIMG(int Idx)
{
	IMG_Key->SetBrushFromMaterial(Keys[Idx]);

	IMG_Key->SetVisibility(ESlateVisibility::Hidden);  
}

void UDoorWidget::InteractionHandle()
{
	PlayAnimation(Interaction); 
}

void UDoorWidget::HideUI()
{
	PlayAnimation(Hide); 
}

void UDoorWidget::ShowUI()
{
	PlayAnimation(Show); 
}
