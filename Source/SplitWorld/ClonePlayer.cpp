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
	
	BodyComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	BodyComp->SetupAttachment(MeshComp);

	BodyComp->SetRelativeLocation(FVector(0, 0, -90));
	BodyComp->SetRelativeRotation(FRotator(0, -90, 0));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempBodyComp
	(TEXT("/Script/Engine.SkeletalMesh'/Game/Slay/Assets/Chr/Echo/Meshes/SK_Echo.SK_Echo'"));

	if (tempBodyComp.Succeeded())
	{
		BodyComp->SetSkeletalMesh(tempBodyComp.Object);
	}
	
	HairComp = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	HairComp->SetupAttachment(BodyComp);

	ConstructorHelpers::FObjectFinder<UGroomAsset>tempHairGroomAsset
	(TEXT("/Script/HairStrandsCore.GroomAsset'/Game/Slay/Assets/Chr/Echo/Hair/Hair_S_UpdoBuns.Hair_S_UpdoBuns'"));

	ConstructorHelpers::FObjectFinder<UGroomBindingAsset>tempHairBindingAsset
	(TEXT("/Script/HairStrandsCore.GroomBindingAsset'/Game/Slay/Assets/Chr/Echo/Hair/Hair_S_UpdoBuns_Echo_M3D_LOD0_Binding.Hair_S_UpdoBuns_Echo_M3D_LOD0_Binding'"));

	HairComp->SetGroomAsset(tempHairGroomAsset.Object);
	HairComp->SetBindingAsset(tempHairBindingAsset.Object);
	
	EyebrowsComp = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	EyebrowsComp->SetupAttachment(BodyComp);

	ConstructorHelpers::FObjectFinder<UGroomAsset>tempEyebrowsGroomAsset
	(TEXT("/Script/HairStrandsCore.GroomAsset'/Game/Slay/Assets/Chr/Echo/Hair/Eyebrows_L_Echo.Eyebrows_L_Echo'"));

	ConstructorHelpers::FObjectFinder<UGroomBindingAsset>tempEyebrowsBindingAsset
	(TEXT("/Script/HairStrandsCore.GroomBindingAsset'/Game/Slay/Assets/Chr/Echo/Hair/Eyebrows_L_Echo_Echo_M3D_LOD0_Binding.Eyebrows_L_Echo_Echo_M3D_LOD0_Binding'"));
	
	EyebrowsComp->SetGroomAsset(tempEyebrowsGroomAsset.Object);
	EyebrowsComp->SetBindingAsset(tempEyebrowsBindingAsset.Object);
}

void AClonePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	anim = Cast<USplitPlayerAnimInstance>(BodyComp->GetAnimInstance());
}

void AClonePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
