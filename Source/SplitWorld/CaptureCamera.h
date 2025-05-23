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
	
private:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere)
	class USceneCaptureComponent2D* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int CameraIdx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class APawn* Player1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class APawn* Player2;
	
};
