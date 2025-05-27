// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ClonePlayer.generated.h"

UCLASS()
class SPLITWORLD_API AClonePlayer : public ACharacter
{
	GENERATED_BODY()

public: 
	AClonePlayer(); 
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
