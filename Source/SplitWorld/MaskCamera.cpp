// Fill out your copyright notice in the Description page of Project Settings.


#include "MaskCamera.h"

#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

AMaskCamera::AMaskCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SetRootComponent(SpringArmComp);
	SpringArmComp->TargetArmLength = 1500.0f; 
	
	CameraComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeRotation(FRotator(27.5f, 0.0f, 0.0f));
	CameraComp->SetIsReplicated(true);
	CameraComp->bEnableClipPlane = true; 

	SetReplicates(true); 
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
}

void AMaskCamera::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FRotator(-90.0f, 0.0f, 0.0f)); 
}

void AMaskCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USceneCaptureComponent2D* AMaskCamera::GetCameraComp()
{
	return CameraComp; 
}
