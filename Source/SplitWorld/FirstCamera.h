// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirstCamera.generated.h" 

USTRUCT() 
struct FCameraData
{
	GENERATED_BODY() 

public: 
	UPROPERTY(EditAnywhere) 
	FVector Location; 
	UPROPERTY(EditAnywhere)
	FRotator Rotation;
	UPROPERTY(EditAnywhere)
	float Length;

};

UCLASS()
class SPLITWORLD_API AFirstCamera : public AActor
{
	GENERATED_BODY()
	
public: 
	AFirstCamera(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 
	
private: 
	void UpdateMask(float DeltaTime);
	void FindPlayers(); 
	void CalcPlayerScreenLocation(); 
	void SetCameraLocation(float DeltaTime);
	void CameraTransformSync(); 
	void ChangePart(); 
	
private: 
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere)
	class USceneCaptureComponent2D* CameraComp; 
	UPROPERTY(EditAnywhere)
	class USceneCaptureComponent2D* MaskComp;
	UPROPERTY(EditAnywhere)
	class USceneCaptureComponent2D* BoundaryComp; 

private: 
	UPROPERTY(EditAnywhere) 
	TSubclassOf<class ASecondCamera> SecondCameraFactory; 
	UPROPERTY(Replicated) 
	class ASecondCamera* SecondCamera; 

	UPROPERTY(EditAnywhere) 
	FVector LocationOffset; 

	UPROPERTY(Replicated)
	FVector2D ScreenAvgPos; 
	FVector2D PlayerScreenLocation[2]; 

private: 
	UPROPERTY() 
	class ASplitWorldGameModeBase* GM; 
	
	UPROPERTY(EditAnywhere, Replicated)
	class APawn* Player1;
	UPROPERTY(EditAnywhere, Replicated)
	class APawn* Player2; 
	
private: 
	UPROPERTY(EditAnywhere) 
	TArray<FCameraData> CameraDatas; 

	float ViewChangePercent; 
	
};
