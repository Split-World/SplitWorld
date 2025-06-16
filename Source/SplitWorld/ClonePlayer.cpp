// Fill out your copyright notice in the Description page of Project Settings.


#include "ClonePlayer.h"

#include "GroomComponent.h"
#include "SplitPlayerAnimInstance.h"

class UGroomBindingAsset;

AClonePlayer::AClonePlayer()
{ 
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true); 
	SetReplicateMovement(true); 
	bAlwaysRelevant = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMeshComp
	(TEXT("/Script/Engine.SkeletalMesh'/Game/Slay/Assets/Chr/Echo/Meshes/SK_Echo.SK_Echo'"));

	if (tempMeshComp.Succeeded())
	{
		MeshComp->SetSkeletalMesh(tempMeshComp.Object);
	}
}

void AClonePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	anim = Cast<USplitPlayerAnimInstance>(MeshComp->GetAnimInstance());
}

void AClonePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
