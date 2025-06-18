// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "GameFramework/Actor.h"
#include "Laser.generated.h"

UCLASS()
class SPLITWORLD_API ALaser : public ATrap
{
	GENERATED_BODY()
	
public:	
	ALaser(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Execute() override;

	UFUNCTION(NetMulticast, Reliable) 
	void Disable(); 

private:
	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* LaserComp; 

};
