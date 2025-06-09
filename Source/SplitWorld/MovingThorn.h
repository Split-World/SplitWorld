// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "Trap.h" 
#include "MovingThorn.generated.h"

UCLASS()
class SPLITWORLD_API AMovingThorn : public ATrap
{
	GENERATED_BODY()
	
public:	
	AMovingThorn(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

	virtual void Execute() override; 

	UPROPERTY(EditAnywhere) 
	class UStaticMeshComponent* Mesh; 

	UPROPERTY(EditAnywhere, Replicated) 
	int Idx; 

	UPROPERTY(EditAnywhere)
	float Speed; 

};
