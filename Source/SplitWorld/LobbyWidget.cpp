// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h" 
#include "Components/Button.h"

void ULobbyWidget::NativeConstruct()
{ 
	Btn_Play->OnClicked.AddDynamic(this, &ULobbyWidget::Play); 
}

void ULobbyWidget::Play()
{ 

}
