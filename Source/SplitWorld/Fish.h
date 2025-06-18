// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fish.generated.h"

UCLASS()
class SPLITWORLD_API AFish : public AActor
{
	GENERATED_BODY()
	
public: 
	AFish();
	UFUNCTION()
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void Launch();

	UFUNCTION(NetMulticast, Reliable)
	void PlaySound(USoundBase* Sound); 

private:
	UFUNCTION() 
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY()
	class ASplitWorldGameModeBase* GM;

	UPROPERTY(Replicated) 
	bool bLaunched; 
	FVector StartLocation;

	UFUNCTION(Server, Reliable)
	void UpdateSuccess(bool bActive);

	UPROPERTY(EditAnywhere)
	class USoundBase* FailSound;
	UPROPERTY(EditAnywhere)
	class USoundBase* LaunchSound; 
	 
};
