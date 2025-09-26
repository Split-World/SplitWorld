// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Snake.generated.h"

UCLASS()
class SPLITWORLD_API ASnake : public AActor
{
	GENERATED_BODY()
	
public:	
	ASnake(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

	UPROPERTY(Replicated) 
	bool bActive; 

private: 
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh; 
	 
};
