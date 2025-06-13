// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorBase.h"

#include "FirstCamera.h"
#include "SecondCamera.h"
#include "SplitPlayer.h"
#include "Components/BoxComponent.h"
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
	InteractableRangeComp->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActorBase::OnInteractableRangeBeginOverlap);
	InteractableRangeComp->OnComponentEndOverlap.AddDynamic(this, &AInteractableActorBase::OnInteractableRangeEndOverlap); 
	
	InteractionWidget = InteractionWidgetComp->GetWidget();
	InteractionWidget->SetVisibility(ESlateVisibility::Hidden); 
}

void AInteractableActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Camera) && IsValid(Camera->GetSecondCamera()))
	{
		FVector Dir = ((!Map ? Camera->GetCameraLocation() : Camera->GetSecondCamera()->GetCameraLocation()) - InteractionWidgetComp->GetComponentLocation()).GetSafeNormal(); 
		InteractionWidgetComp->SetRelativeRotation(Dir.Rotation());
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

	InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
	Server_SetVisibleUI(ESlateVisibility::Hidden); 
}

void AInteractableActorBase::OnVisibleUIRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AInteractableActorBase::OnInteractableRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor->IsA<ASplitPlayer>()) return; 
	if (!HasAuthority()) return; 
	if (!Idx && !Cast<ASplitPlayer>(OtherActor)->IsLocallyControlled()) return; 
	if (Idx && Cast<ASplitPlayer>(OtherActor)->IsLocallyControlled()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s"), *UEnum::GetValueAsString(GetLocalRole()))); 
	InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	Server_SetVisibleUI(ESlateVisibility::Visible); 
}

void AInteractableActorBase::OnInteractableRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->IsA<ASplitPlayer>()) return; 
	
	InteractionWidget->SetVisibility(ESlateVisibility::Hidden); 
	Server_SetVisibleUI(ESlateVisibility::Hidden); 
}

void AInteractableActorBase::Server_SetVisibleUI_Implementation(ESlateVisibility Visible)
{
	Multi_SetVisibleUI(Visible); 
}

void AInteractableActorBase::Multi_SetVisibleUI_Implementation(ESlateVisibility Visible)
{
	InteractionWidget->SetVisibility(Visible); 
} 
