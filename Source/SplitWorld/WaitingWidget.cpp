// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingWidget.h"
#include "SplitWorldGameInstance.h" 
#include "Components/Button.h" 
#include "GameFramework/GameStateBase.h" 
#include "Components/Border.h" 

void UWaitingWidget::NativeConstruct()
{
	Super::NativeConstruct(); 
	
	GI = Cast<USplitWorldGameInstance>(GetWorld()->GetGameInstance()); 
	GS = Cast<AGameStateBase>(GetWorld()->GetGameState()); 

	BTN_ConnectionBreak->OnClicked.AddDynamic(this, &UWaitingWidget::ConnectionBreak); 
	BTN_GameStart->OnClicked.AddDynamic(this, &UWaitingWidget::GameStart); 

	BTN_GameStart->SetIsEnabled(false); 
}

void UWaitingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{ 
	Super::NativeTick(MyGeometry, InDeltaTime); 
	
	bool bIsFullRoom = GS->PlayerArray.Num() == 2; 
	BTN_GameStart->SetIsEnabled(bIsFullRoom); 

	BD_Player2->SetBrushColor(FLinearColor(BD_Player1->GetBrushColor() * (bIsFullRoom ? 1.0f : 0.5f))); 
} 

void UWaitingWidget::ConnectionBreak()
{ 
	if (GI)
	{ 
		GI->ExitRoom(); 
	} 
}

void UWaitingWidget::GameStart()
{ 
	if (GI)
	{
		GI->GameStart(); 
	}
}
