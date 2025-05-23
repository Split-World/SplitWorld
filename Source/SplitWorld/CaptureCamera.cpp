// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureCamera.h"

#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

ACaptureCamera::ACaptureCamera()
{
 	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SetRootComponent(SpringArmComp);
	SpringArmComp->TargetArmLength = 500.0f; 

	CameraComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeRotation(FRotator(20.0f, 0.0f, 0.0f)); 
}

void ACaptureCamera::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FRotator(-90.0f, 0.0f, 0.0f)); 
} 

void ACaptureCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
