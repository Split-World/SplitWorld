// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class SPLITWORLD_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADoor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private: 
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY()
	class ASplitWorldGameModeBase* GM;

	FVector StartLocation; 
	
};
