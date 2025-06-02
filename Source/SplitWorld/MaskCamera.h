// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaskCamera.generated.h"

UCLASS()
class SPLITWORLD_API AMaskCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	AMaskCamera(); 
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;

	class USceneCaptureComponent2D* GetCameraComp(); 

private:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere)
	class USceneCaptureComponent2D* CameraComp;
	
};
