// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorHandleDevice.h" 
#include "SplitWorldGameModeBase.h" 
#include "Door.h" 
#include "DoorHandle.h"
#include "MovieSceneTracksComponentTypes.h"

ADoorHandleDevice::ADoorHandleDevice()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	MeshComp->SetCollisionProfileName(TEXT("Objects"));
	MeshComp->SetRelativeScale3D(FVector(3.5f, 3.5f, 1.5f)); 
	MeshComp->SetIsReplicated(true);

	Handle1_PointComp = CreateDefaultSubobject<USceneComponent>(TEXT("Handle1_PointComp"));
	Handle1_PointComp->SetupAttachment(MeshComp);
	Handle1_PointComp->SetRelativeLocation(FVector(0, 110.0f, 0)); 
	Handle1_PointComp->SetIsReplicated(true); 

	Handle2_PointComp = CreateDefaultSubobject<USceneComponent>(TEXT("Handle2_PointComp"));
	Handle2_PointComp->SetupAttachment(MeshComp);
	Handle2_PointComp->SetRelativeLocation(FVector(0, -110.0f, 0)); 
	Handle2_PointComp->SetRelativeRotation(FRotator(0, 180.0f, 0)); 
	Handle2_PointComp->SetIsReplicated(true); 

	SetReplicates(true);
	SetReplicateMovement(true); 
	bAlwaysRelevant = true;
}

void ADoorHandleDevice::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	DoorHandles[0]->AttachToComponent(Handle1_PointComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);  
	DoorHandles[1]->AttachToComponent(Handle2_PointComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);  
} 

void ADoorHandleDevice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && GM->DoorInput) 
	{ 
		float Yaw = FMath::Lerp(0, 450.0f, GM->DoorGauge / 10.0f); 
		SetActorRotation(FRotator(0, Yaw, 0.0f)); 
	}  
} 

