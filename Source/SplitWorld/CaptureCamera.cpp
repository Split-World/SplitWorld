// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureCamera.h" 
#include "ClonePlayer.h"
#include "MaskCamera.h"
#include "SplitWorldGameModeBase.h"
#include "TempPlayer.h"
#include "Engine/SceneCapture2D.h" 
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Net/UnrealNetwork.h" 

ACaptureCamera::ACaptureCamera()
{
 	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SetRootComponent(SpringArmComp);
	SpringArmComp->TargetArmLength = 3500.0f; 

	CameraComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeRotation(FRotator(27.5f, 0.0f, 0.0f)); 

	MaskComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MaskComp"));
	MaskComp->SetupAttachment(SpringArmComp);
	MaskComp->SetRelativeRotation(FRotator(27.5f, 0.0f, 0.0f)); 

	SetReplicates(true); 
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
} 

void ACaptureCamera::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FRotator(-90.0f, 0.0f, 0.0f));

	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	} 

	if (!CameraIdx)
	{
		MaskComp->bEnableClipPlane = true; 
	} 
} 

void ACaptureCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	
	if (IsValid(Player1) && IsValid(Player2))
	{ 
		CalcPlayerScreenLocation(); 
		SetCameraLocation(); 
		UpdateMask(); 
	} 
	else
	{
		FindPlayers(); 
	}
} 

void ACaptureCamera::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACaptureCamera, CameraComp); 
	DOREPLIFETIME(ACaptureCamera, MaskComp); 
	DOREPLIFETIME(ACaptureCamera, CameraIdx); 
	DOREPLIFETIME(ACaptureCamera, Player1); 
	DOREPLIFETIME(ACaptureCamera, Player2); 
}

//void ACaptureCamera::UpdateMask_Implementation()
void ACaptureCamera::UpdateMask()
{ 
	if (!CameraIdx)
	{
		FVector P1 = Player1->GetActorLocation();
		FVector P2 = Player2->GetActorLocation();
		FVector Dist = P1 - P2;
		FVector AvgPos = Dist * 0.5f + P2;
		
		MaskComp->ClipPlaneBase = AvgPos;

		if (Dist.Size2D() < 300.0f && Dist.Z > 1000.0f)
		{
			MaskComp->ClipPlaneNormal = -Dist.GetSafeNormal2D();
		}
		else
		{
			MaskComp->ClipPlaneNormal = -Dist.GetSafeNormal();
		} 
	} 
}

void ACaptureCamera::FindPlayers_Implementation()
{ 
	if (GM->Players.Num() != 2)
	{
		return; 
	}

	auto P1 = Cast<ATempPlayer>(GM->Players[0]->GetPawn()); 
	auto P2 = Cast<ATempPlayer>(GM->Players[1]->GetPawn());
	if (!IsValid(P1->Clone) || !IsValid(P2->Clone))
	{
		return; 
	}

	if (!CameraIdx)
	{
		Player1 = P1; 
		Player2 = P2->Clone; 
	}
	else 
	{
		Player1 = P1->Clone; 
		Player2 = P2; 
	} 
} 

void ACaptureCamera::CalcPlayerScreenLocation_Implementation()
{
	FVector pos = CameraComp->GetComponentLocation();
	FVector r = CameraComp->GetRightVector();
	FVector u = CameraComp->GetUpVector();
	FVector f = CameraComp->GetForwardVector();
	float px = 0;
	float py = 0;
	if (!CameraIdx)
	{ 
		FVector pp = Player1->GetActorLocation();
		float z = f.Dot(Player1->GetActorLocation() - pos);
		float vx = r.X * pp.X + r.Y * pp.Y + r.Z * pp.Z - r.Dot(pos);
		float vy = u.X * pp.X + u.Y * pp.Y + u.Z * pp.Z - u.Dot(pos);
		px = vx / z;
		py = vy / z;
	}
	else 
	{ 
		FVector pp = Player2->GetActorLocation();
		float z = f.Dot(Player2->GetActorLocation() - pos);
		float vx = r.X * pp.X + r.Y * pp.Y + r.Z * pp.Z - r.Dot(pos);
		float vy = u.X * pp.X + u.Y * pp.Y + u.Z * pp.Z - u.Dot(pos);
		px = vx / z;
		py = vy / z; 
	}
	GM->PlayerScreenLocation[CameraIdx] = FVector2D(px, py); 
}

void ACaptureCamera::SetCameraLocation_Implementation()
{ 
	FVector2D P1 = GM->PlayerScreenLocation[0]; 
	FVector2D P2 = GM->PlayerScreenLocation[1]; 
	float Min_X = FMath::Min(P1.X, P2.X); 
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%f"), Min_X)); 
	if (Min_X < -0.5f) 
	{ 
		SetActorLocation(GetActorLocation() + FVector(0, -1, 0) * 500.0f * GetWorld()->GetDeltaSeconds()); 
	} 
	else if (Min_X > -0.15f) 
	{
		SetActorLocation(GetActorLocation() + FVector(0, 1, 0) * 500.0f * GetWorld()->GetDeltaSeconds()); 
	} 
}
