// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecondCamera.generated.h"

class USpringArmComponent;

UCLASS()
class SPLITWORLD_API ASecondCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	ASecondCamera(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	FVector GetCameraLocation();

	USpringArmComponent* GetSpringArm(); 

private: 
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere)
	class USceneCaptureComponent2D* CameraComp; 

};
