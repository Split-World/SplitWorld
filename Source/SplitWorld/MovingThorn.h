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

	UPROPERTY(Replicated) 
	int Idx; 

	UFUNCTION(Server, Reliable)
	void Server_SetMesh(int _Idx);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_SetMesh(int _Idx); 

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	TArray<class UStaticMesh*> Meshes;

	UPROPERTY(EditAnywhere)
	TArray<class UMaterialInstance*> Materials;

};
