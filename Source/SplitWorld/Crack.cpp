// Fill out your copyright notice in the Description page of Project Settings.


#include "Crack.h" 

#include "CrackWidget.h"
#include "FirstCamera.h"
#include "SecondCamera.h"
#include "Components/BoxComponent.h" 
#include "SplitWorldGameModeBase.h" 
#include "Components/SceneCaptureComponent2D.h"
#include "Components/WidgetComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

ACrack::ACrack()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp->SetCollisionProfileName(TEXT("Crack"));
	BoxComp->SetIsReplicated(true); 

	CrackWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("CrackWidgetComp")); 
	CrackWidgetComp->SetupAttachment(BoxComp);
	CrackWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	CrackWidgetComp->SetDrawSize(FVector2D(150.0f)); 
	
	bReplicates = true; 
	bAlwaysRelevant = true; 
}

void ACrack::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
		GM->ChangePartDelegate.AddLambda([&](){ ChangePart(); }); 
	}

	MPC_Instance = GetWorld()->GetParameterCollectionInstance(MPC_SplitWorld); 

	CrackWidget = Cast<UCrackWidget>(CrackWidgetComp->GetWidget());
	CrackWidget->SetIMG(Idx); 
}

void ACrack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Camera) && IsValid(Camera->GetSecondCamera()))
	{
		FVector Forward = GetActorForwardVector(); 
		FVector CameraForward = !Map ? Camera->GetCamera()->GetForwardVector() : Camera->GetSecondCamera()->GetCamera()->GetForwardVector(); 
		
		FVector CameraPos = !Map ? Camera->GetCamera()->GetComponentLocation() : Camera->GetSecondCamera()->GetCamera()->GetComponentLocation(); 
		FVector Dir = (CameraPos - CrackWidgetComp->GetComponentLocation()).GetSafeNormal(); 
		float RotDir = FMath::Sign(Forward.Cross(-CameraForward.GetSafeNormal2D()).Z); 
		float Yaw = FMath::RadiansToDegrees(FMath::Acos(Forward.Dot(-CameraForward.GetSafeNormal2D()))) * RotDir; 
		
		FRotator Rot = Dir.Rotation();
		Rot.Yaw = Yaw;
		CrackWidgetComp->SetRelativeRotation(Rot);
	}

	if (GM)
	{
		UpdateCrackGauge(GM->CrackGauge[0], GM->CrackGauge[1]); 
	}
}

void ACrack::Interaction_Implementation()
{ 
	Super::Interaction_Implementation(); 

	if (GM->bPlayer_Interactions[3] == 3)
	{ 
		GM->CrackInput |= (1 << Idx); 

		GetWorldTimerManager().ClearTimer(CrackTimerHandle);
		GetWorldTimerManager().SetTimer(CrackTimerHandle, [&]()
		{
			GM->CrackInput &= ~(1 << Idx);
		}, 0.05f, false); 
	}
	
	if (!(GM->bPlayer_Interactions[3] & (1 << Idx)))
	{
		GM->bPlayer_Interactions[3] |= (1 << Idx);
		Multi_SetVisibility(); 
	}
}

void ACrack::ChangePart()
{
	if (GM->CurPart == EMapPart::PartCrack)
	{
		ShowUI(); 
	}
}

void ACrack::UpdateCrackGauge_Implementation(float Pink, float Green)
{
	if (MPC_Instance)
	{
		MPC_Instance->SetScalarParameterValue(FName(TEXT("CircularPercent_Pink")), Pink); 
		MPC_Instance->SetScalarParameterValue(FName(TEXT("CircularPercent_Green")), Green);
	} 
}

void ACrack::ShowUI_Implementation()
{
	CrackWidget->ShowCrackUI(); 
}

