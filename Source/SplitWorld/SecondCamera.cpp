// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondCamera.h" 
#include "GameFramework/SpringArmComponent.h" 
#include "Components/SceneCaptureComponent2D.h" 

ASecondCamera::ASecondCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SetRootComponent(SpringArmComp);
	SpringArmComp->TargetArmLength = 3000.0f;
	SpringArmComp->SetIsReplicated(true);

	CameraComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeRotation(FRotator(12.5f, 0.0f, 0.0f));
	CameraComp->SetIsReplicated(true); 
	
	SetReplicates(true);
	SetReplicateMovement(true);
	bAlwaysRelevant = true;
}

void ASecondCamera::BeginPlay()
{
	Super::BeginPlay();

}

void ASecondCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASecondCamera::GetCameraLocation()
{
	return CameraComp->GetComponentLocation(); 
}

