// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "Trap.h" 
#include "FireLaser.generated.h"

UCLASS()
class SPLITWORLD_API AFireLaser : public ATrap
{
	GENERATED_BODY()
	
public: 
	AFireLaser(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Execute() override; 

	void Fire(); 

private: 
	UPROPERTY(EditAnywhere) 
	class UNiagaraComponent* LaserComp; 
	
};
