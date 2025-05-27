// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TempPlayer.generated.h"

UCLASS()
class SPLITWORLD_API ATempPlayer : public ACharacter
{
	GENERATED_BODY()

public: 
	ATempPlayer(); 
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated) 
	class AClonePlayer* Clone; 

};
