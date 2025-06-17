// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorHandle.h" 

#include "DoorWidget.h"
#include "FirstCamera.h"
#include "SecondCamera.h"
#include "SplitWorldGameModeBase.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/WidgetComponent.h"

ADoorHandle::ADoorHandle()
{ 
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxComp);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	Player_PointComp = CreateDefaultSubobject<USceneComponent>(TEXT("Player_PointComp"));
	Player_PointComp->SetupAttachment(RootComponent);
	Player_PointComp->SetRelativeLocation(FVector(30.0f, 0, 0)); 
	Player_PointComp->SetRelativeRotation(FRotator(0, 180.0f, 0)); 
	Player_PointComp->SetIsReplicated(true); 

	DoorWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("DoorWidgetComp")); 
	DoorWidgetComp->SetupAttachment(BoxComp);
	DoorWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	DoorWidgetComp->SetDrawSize(FVector2D(100.0f)); 
	
	bReplicates = true;
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
}

void ADoorHandle::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
		GM->ChangePartDelegate.AddLambda([&](){ ChangePart(); }); 
	}

	DoorWidget = Cast<UDoorWidget>(DoorWidgetComp->GetWidget());
	DoorWidget->SetIMG(Idx); 
}

void ADoorHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Camera) && IsValid(Camera->GetSecondCamera()))
	{
		UILookCamera(); 
	}
}

void ADoorHandle::Interaction_Implementation()
{
	Super::Interaction_Implementation();
	
	if (GM->bPlayer_Interactions[0] == 3) 
	{ 
		GM->DoorInput |= (1 << Idx);

		Multi_Interaction(); 
		
		GetWorldTimerManager().ClearTimer(DoorInputTimerHandle); 
		GetWorldTimerManager().SetTimer(DoorInputTimerHandle, [&]()
		{
			GM->DoorInput &= ~(1 << Idx); 
		}, 0.1f, false); 
	}
	
	if (!(GM->bPlayer_Interactions[0] & (1 << Idx)))
	{
		GM->bPlayer_Interactions[0] |= (1 << Idx); 
		GM->Players[Idx]->GetPawn()->AttachToComponent(Player_PointComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Multi_SetVisibility(); 
	} 
}

void ADoorHandle::ChangePart()
{
	if (GM->CurPart == EMapPart::Part2)
	{
		GM->Players[Idx]->GetPawn()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); 
		Multi_HideDoorUI(); 
	}
	else if (GM->CurPart == EMapPart::PartDoor)
	{
		Multi_ShowDoorUI(); 
	}
}

void ADoorHandle::UILookCamera()
{
	FVector Forward = GetActorForwardVector(); 
	FVector CameraForward = !Map ? Camera->GetCamera()->GetForwardVector() : Camera->GetSecondCamera()->GetCamera()->GetForwardVector(); 
		
	FVector CameraPos = !Map ? Camera->GetCamera()->GetComponentLocation() : Camera->GetSecondCamera()->GetCamera()->GetComponentLocation(); 
	FVector Dir = (CameraPos - DoorWidgetComp->GetComponentLocation()).GetSafeNormal(); 
	float RotDir = FMath::Sign(Forward.Cross(-CameraForward.GetSafeNormal2D()).Z); 
	float Yaw = FMath::RadiansToDegrees(FMath::Acos(Forward.Dot(-CameraForward.GetSafeNormal2D()))) * RotDir; 
		
	FRotator Rot = Dir.Rotation();
	Rot.Yaw = Yaw;
	DoorWidgetComp->SetRelativeRotation(Rot); 
}

void ADoorHandle::Multi_ShowDoorUI_Implementation()
{
	DoorWidget->ShowUI(); 
} 

void ADoorHandle::Multi_HideDoorUI_Implementation()
{
	DoorWidget->HideUI(); 
}

void ADoorHandle::Multi_Interaction_Implementation()
{
	DoorWidget->InteractionHandle(); 	
}
