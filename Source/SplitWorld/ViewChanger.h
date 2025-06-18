// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplitWorldGameModeBase.h" 
#include "ViewChanger.generated.h"

UCLASS()
class SPLITWORLD_API AViewChanger : public AActor
{
	GENERATED_BODY()
	
public:	
	AViewChanger(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

private: 
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere) 
	class UBoxComponent* BoxComp;

	UPROPERTY()
	class ASplitWorldGameModeBase* GM;

	UPROPERTY(EditAnywhere)
	int Idx; 

	UPROPERTY(EditAnywhere)
	EMapPart ChangeView; 
	
};
