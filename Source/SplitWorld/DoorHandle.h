// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "InteractableActorBase.h"
#include "DoorHandle.generated.h"

UCLASS()
class SPLITWORLD_API ADoorHandle : public AInteractableActorBase 
{
	GENERATED_BODY()
	
public: 
	ADoorHandle(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction_Implementation() override; 

private:
	UFUNCTION(NetMulticast, Reliable)
	void Multi_ShowDoorUI();
	UFUNCTION(NetMulticast, Reliable)
	void Multi_HideDoorUI(); 
	UFUNCTION(NetMulticast, Reliable)
	void Multi_Interaction();

	UFUNCTION()
	void ChangePart(); 

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	class USceneComponent* Player_PointComp;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* DoorWidgetComp;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* Arrow1WidgetComp;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* Arrow2WidgetComp;
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* Arrow3WidgetComp;
	
	UPROPERTY()
	class ASplitWorldGameModeBase* GM; 

	FTimerHandle DoorInputTimerHandle;

	UPROPERTY(EditAnywhere)
	class UDoorWidget* DoorWidget; 
	UPROPERTY(EditAnywhere)
	class UArrowWidget* Arrow1Widget;
	UPROPERTY(EditAnywhere)
	class UArrowWidget* Arrow2Widget;
	UPROPERTY(EditAnywhere)
	class UArrowWidget* Arrow3Widget;

}; 
