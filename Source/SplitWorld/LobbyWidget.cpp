// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h" 

#include "SplitWorldGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void ULobbyWidget::NativeConstruct()
{
	GI = Cast<USplitWorldGameInstance>(GetWorld()->GetGameInstance()); 
	Btn_Play->OnClicked.AddDynamic(this, &ULobbyWidget::Play); 
	Btn_Quit->OnClicked.AddDynamic(this, &ULobbyWidget::Quit); 
}

void ULobbyWidget::Play()
{ 
	if (GI)
	{
		GI->FindSession();
		Btn_Play->SetIsEnabled(false); 
	}
}

void ULobbyWidget::Quit()
{
	auto pc = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), pc, EQuitPreference::Quit, false); 
}
