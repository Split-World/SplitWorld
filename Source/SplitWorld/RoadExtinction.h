// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadExtinction.generated.h"

UCLASS()
class SPLITWORLD_API ARoadExtinction : public AActor
{
	GENERATED_BODY()
	
public: 
	ARoadExtinction(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override; 

private: 
	void OnStartBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); 
	void OnDestroyBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); 

	UPROPERTY(EditAnywhere) 
	class USceneComponent* Root; 
	UPROPERTY(EditAnywhere) 
	class UBoxComponent* StartBoxComp; 
	UPROPERTY(EditAnywhere)
	class UBoxComponent* DestroyBoxComp; 

	UPROPERTY(EditAnywhere) 
	bool bStart; 

};
