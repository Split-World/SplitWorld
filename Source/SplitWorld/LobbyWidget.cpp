// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h" 

#include "SplitWorldGameInstance.h"
#include "Components/Button.h"

void ULobbyWidget::NativeConstruct()
{
	GI = Cast<USplitWorldGameInstance>(GetWorld()->GetGameInstance()); 
	Btn_Play->OnClicked.AddDynamic(this, &ULobbyWidget::Play); 
}

void ULobbyWidget::Play()
{ 
	if (GI)
	{
		GI->FindSession();
		Btn_Play->SetIsEnabled(false); 
	}
}
