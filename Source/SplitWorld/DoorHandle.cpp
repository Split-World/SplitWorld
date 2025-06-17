// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorHandle.h" 

#include "DoorWidget.h"
#include "FirstCamera.h"
#include "SecondCamera.h"
#include "SplitWorldGameModeBase.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/WidgetComponent.h"
#include "ArrowWidget.h"

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

	Arrow1WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Arrow1WidgetComp"));
	Arrow1WidgetComp->SetupAttachment(DoorWidgetComp);
	Arrow1WidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	Arrow1WidgetComp->SetDrawSize(FVector2D(100.0f)); 

	Arrow2WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Arrow2WidgetComp"));
	Arrow2WidgetComp->SetupAttachment(DoorWidgetComp);
	Arrow2WidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f)); 
	Arrow2WidgetComp->SetDrawSize(FVector2D(100.0f));

	Arrow3WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Arrow3WidgetComp"));
	Arrow3WidgetComp->SetupAttachment(DoorWidgetComp);
	Arrow3WidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	Arrow3WidgetComp->SetDrawSize(FVector2D(100.0f));
	
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
	Arrow1Widget = Cast<UArrowWidget>(Arrow1WidgetComp->GetWidget());
	Arrow1Widget->SetIMG();
	Arrow2Widget = Cast<UArrowWidget>(Arrow2WidgetComp->GetWidget());
	Arrow2Widget->SetIMG();
	Arrow3Widget = Cast<UArrowWidget>(Arrow3WidgetComp->GetWidget());
	Arrow3Widget->SetIMG(); 
}

void ADoorHandle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Camera) && IsValid(Camera->GetSecondCamera()))
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

void ADoorHandle::Multi_ShowDoorUI_Implementation()
{
	DoorWidget->ShowUI(); 
	Arrow1Widget->ShowUI(); 
	Arrow2Widget->ShowUI(); 
	Arrow3Widget->ShowUI(); 
} 

void ADoorHandle::Multi_HideDoorUI_Implementation()
{
	DoorWidget->HideUI(); 
	Arrow1Widget->HideUI(); 
	Arrow2Widget->HideUI(); 
	Arrow3Widget->HideUI(); 
}

void ADoorHandle::Multi_Interaction_Implementation()
{
	DoorWidget->InteractionHandle(); 
}
