// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorBase.h"

#include "FirstCamera.h"
#include "InteractionWidget.h"
#include "SecondCamera.h"
#include "SplitPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h" 

AInteractableActorBase::AInteractableActorBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetCollisionProfileName(TEXT("Objects"));
	BoxComp->SetBoxExtent(FVector(50.0f)); 
	
	InteractionWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidgetComp")); 
	InteractionWidgetComp->SetupAttachment(BoxComp);
	InteractionWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	InteractionWidgetComp->SetDrawSize(FVector2D(100.0f)); 

	ConstructorHelpers::FClassFinder<UUserWidget> TempUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SK/UI/WBP_Interaction.WBP_Interaction_C'")); 
	if (TempUI.Succeeded())
	{
		InteractionWidgetComp->SetWidgetClass(TempUI.Class); 
	}
	
	VisibleUIRangeComp = CreateDefaultSubobject<USphereComponent>(TEXT("VisibleUIRangeComp")); 
	VisibleUIRangeComp->SetupAttachment(BoxComp);
	VisibleUIRangeComp->SetSphereRadius(500.0f); 
	
	InteractableRangeComp = CreateDefaultSubobject<USphereComponent>(TEXT("InteractableRangeComp")); 
	InteractableRangeComp->SetupAttachment(BoxComp);
	InteractableRangeComp->SetSphereRadius(150.0f); 
}

void AInteractableActorBase::BeginPlay()
{
	Super::BeginPlay();

	VisibleUIRangeComp->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActorBase::OnVisibleUIRangeBeginOverlap); 
	VisibleUIRangeComp->OnComponentEndOverlap.AddDynamic(this, &AInteractableActorBase::OnVisibleUIRangeEndOverlap); 
	InteractableRangeComp->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActorBase::OnInteractableRangeBeginOverlap);
	InteractableRangeComp->OnComponentEndOverlap.AddDynamic(this, &AInteractableActorBase::OnInteractableRangeEndOverlap); 
	
	InteractionWidget = Cast<UInteractionWidget>(InteractionWidgetComp->GetWidget()); 
	InteractionWidget->SetIMG(Idx); 
}

void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Camera) && IsValid(Camera->GetSecondCamera()))
	{
		FRotator Rot(0, 0, 0);
		FVector Forward = GetActorForwardVector(); 
		FVector CameraForward = !Map ? Camera->GetCamera()->GetForwardVector() : Camera->GetSecondCamera()->GetCamera()->GetForwardVector(); 
		FVector Right = GetActorRightVector();
		FVector Up = GetActorUpVector();

		FVector CameraPos = !Map ? Camera->GetCamera()->GetComponentLocation() : Camera->GetSecondCamera()->GetCamera()->GetComponentLocation(); 
		FVector Dir = (CameraPos - InteractionWidgetComp->GetComponentLocation()).GetSafeNormal(); 
		float RotDir = FMath::Sign(Forward.Cross(-CameraForward.GetSafeNormal2D()).Z); 
		float Yaw = FMath::RadiansToDegrees(FMath::Acos(Forward.Dot(-CameraForward.GetSafeNormal2D()))) * RotDir; 
		
		Rot = Dir.Rotation();
		Rot.Yaw = Yaw; 
		FVector x = Camera->GetCamera()->GetForwardVector().GetSafeNormal2D();
		InteractionWidgetComp->SetRelativeRotation(Rot); 
	} 
}

void AInteractableActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInteractableActorBase, Idx); 
}

void AInteractableActorBase::Interaction_Implementation()
{
	IInteractable::Interaction_Implementation();

	Multi_ExecuteFunction(EFunctionType::DeactiveKey, false); 
}

void AInteractableActorBase::OnVisibleUIRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA<ASplitPlayer>()) return; 
	if (!HasAuthority()) return; 
	if (!Idx && !Cast<ASplitPlayer>(OtherActor)->IsLocallyControlled()) return; 
	if (Idx && Cast<ASplitPlayer>(OtherActor)->IsLocallyControlled()) return;
	
	Multi_ExecuteFunction(EFunctionType::ShowInteraction, false); 
}

void AInteractableActorBase::OnVisibleUIRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->IsA<ASplitPlayer>()) return; 
	if (!HasAuthority()) return; 
	if (!Idx && !Cast<ASplitPlayer>(OtherActor)->IsLocallyControlled()) return; 
	if (Idx && Cast<ASplitPlayer>(OtherActor)->IsLocallyControlled()) return;

	Multi_ExecuteFunction(EFunctionType::HideInteraction, false); 
}

void AInteractableActorBase::OnInteractableRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA<ASplitPlayer>()) return; 
	if (!HasAuthority()) return; 
	if (!Idx && !Cast<ASplitPlayer>(OtherActor)->IsLocallyControlled()) return; 
	if (Idx && Cast<ASplitPlayer>(OtherActor)->IsLocallyControlled()) return;
	
	Multi_ExecuteFunction(EFunctionType::ChangeIMG, true); 
}

void AInteractableActorBase::OnInteractableRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->IsA<ASplitPlayer>()) return; 
	if (!HasAuthority()) return; 
	if (!Idx && !Cast<ASplitPlayer>(OtherActor)->IsLocallyControlled()) return; 
	if (Idx && Cast<ASplitPlayer>(OtherActor)->IsLocallyControlled()) return;

	Multi_ExecuteFunction(EFunctionType::ChangeIMG, false); 
}

void AInteractableActorBase::Multi_ExecuteFunction_Implementation(EFunctionType Type, bool Value)
{
	switch (Type)
	{
	case EFunctionType::ShowInteraction:
		InteractionWidget->ShowInteraction();
		break;
	case EFunctionType::HideInteraction:
		InteractionWidget->HideInteraction();
		break;
	case EFunctionType::ChangeIMG:
		InteractionWidget->ChangeIMG(Value);
		break;
	case EFunctionType::ActiveKey:
		InteractionWidget->ActiveKey();
		break;
	case EFunctionType::DeactiveKey:
		InteractionWidget->DeactiveKey();
		break; 
	}
} 

void AInteractableActorBase::Multi_SetVisibility_Implementation()
{ 
	InteractionWidget->SetVisibility(ESlateVisibility::Hidden); 
}
