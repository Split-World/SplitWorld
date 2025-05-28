// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class SPLITWORLD_API ATrap : public AActor
{
	GENERATED_BODY()
	
public: 
	ATrap(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	virtual void Execute() {}; 

protected: 
	UPROPERTY() 
	class UBoxComponent* BoxComp;

	UPROPERTY()
	class ASplitPlayer* Player; 
	
private: 
	UFUNCTION() 
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); 
	
};
