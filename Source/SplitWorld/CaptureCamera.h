// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaptureCamera.generated.h"

UCLASS()
class SPLITWORLD_API ACaptureCamera : public AActor
{
	GENERATED_BODY()
	
public: 
	ACaptureCamera(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 
	
private: 
	//UFUNCTION(Server, Reliable) 
	void UpdateMask();
	UFUNCTION(Server, Reliable) 
	void FindPlayers();
	UFUNCTION(Server, Reliable) 
	void CalcPlayerScreenLocation(); 
	UFUNCTION(Server, Reliable) 
	void SetCameraLocation(); 

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, Replicated)
	class USceneCaptureComponent2D* CameraComp; 
	UPROPERTY(EditAnywhere, Replicated)
	class USceneCaptureComponent2D* MaskComp;

	UPROPERTY(EditAnywhere, Replicated)
	int CameraIdx;

	UPROPERTY()
	class ASplitWorldGameModeBase* GM; 
	
	UPROPERTY(EditAnywhere, Replicated)
	class APawn* Player1;
	UPROPERTY(EditAnywhere, Replicated)
	class APawn* Player2; 
	
};
