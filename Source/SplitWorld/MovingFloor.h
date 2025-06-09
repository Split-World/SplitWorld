// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingFloor.generated.h"

UCLASS()
class SPLITWORLD_API AMovingFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingFloor(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override; 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY()
	class ASplitWorldGameModeBase* GM;


private:
	UPROPERTY(EditAnywhere)
	AActor* OtherFloor; 
	UPROPERTY(EditAnywhere)
	TArray<class AActor*> Handles;

	UPROPERTY(Replicated)
	float MoveDistance = 7000.0f;

	UPROPERTY(EditAnywhere)
	int Idx; 
	
};
