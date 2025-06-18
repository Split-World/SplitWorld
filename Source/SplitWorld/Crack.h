 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "InteractableActorBase.h"
#include "Crack.generated.h"

UCLASS()
class SPLITWORLD_API ACrack : public AInteractableActorBase 
{
	GENERATED_BODY()
	
public:	
	ACrack(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction_Implementation() override;

	UFUNCTION()
	void ChangePart();

	UFUNCTION(NetMulticast, Reliable)
	void UpdateCrackGauge(float Pink, float Green); 

	UPROPERTY(EditAnywhere)
	class UMaterialParameterCollection* MPC_SplitWorld;
	
private:
	UFUNCTION(NetMulticast, Reliable)
	void ShowUI();
	
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* CrackWidgetComp;
	
	UPROPERTY()
	class ASplitWorldGameModeBase* GM; 

	FTimerHandle CrackTimerHandle;

	UPROPERTY(EditAnywhere)
	class UCrackWidget* CrackWidget;

	UPROPERTY()
	class UMaterialParameterCollectionInstance* MPC_Instance;
 
};
