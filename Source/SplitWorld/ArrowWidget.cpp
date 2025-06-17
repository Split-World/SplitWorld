// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowWidget.h" 
#include "Components/Image.h"

void UArrowWidget::SetIMG()
{
	IMG_Arrow->SetVisibility(ESlateVisibility::Hidden);  
}

void UArrowWidget::HideUI()
{
	PlayAnimation(Hide); 
}

void UArrowWidget::ShowUI()
{
	PlayAnimation(Show);
	PlayAnimation(MoveArrow); 
}
