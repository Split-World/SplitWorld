// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureCamera.h" 
#include "ClonePlayer.h" 
#include "SplitPlayer.h"
#include "SplitWorldGameModeBase.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Net/UnrealNetwork.h" 

ACaptureCamera::ACaptureCamera()
{
 	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SetRootComponent(SpringArmComp);
	SpringArmComp->TargetArmLength = 3500.0f; 
	SpringArmComp->SetIsReplicated(true); 

	CameraComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeRotation(FRotator(12.5f, 0.0f, 0.0f));
	CameraComp->SetIsReplicated(true); 

	MaskComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MaskComp"));
	MaskComp->SetupAttachment(SpringArmComp);
	MaskComp->SetRelativeRotation(FRotator(12.5f, 0.0f, 0.0f)); 
	MaskComp->SetIsReplicated(true); 

	BoundaryComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("BoundaryComp"));
	BoundaryComp->SetupAttachment(SpringArmComp);
	BoundaryComp->SetRelativeRotation(FRotator(12.5f, 0.0f, 0.0f));
	BoundaryComp->SetIsReplicated(true); 

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
		BoundaryComp->bEnableClipPlane = true; 
	} 
} 

void ACaptureCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	
	if (IsValid(Player1) && IsValid(Player2))
	{
		if (HasAuthority())
		{
			CalcPlayerScreenLocation(); 
		}
		
		UpdateMask(); 
	}
	else
	{
		if (HasAuthority())
		{
			FindPlayers();
		}
	}
} 

void ACaptureCamera::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACaptureCamera, CameraIdx); 
	DOREPLIFETIME(ACaptureCamera, Player1); 
	DOREPLIFETIME(ACaptureCamera, Player2); 
	DOREPLIFETIME(ACaptureCamera, ScreenAvgPos); 
}

void ACaptureCamera::UpdateMask()
{ 
	if (!CameraIdx)
	{ 
		FVector P1 = Player1->GetActorLocation();
		FVector P2 = Player2->GetActorLocation();
		FVector Dist = P1 - P2; 

		FVector pos = CameraComp->GetComponentLocation();
		FVector r = CameraComp->GetRightVector();
		FVector u = CameraComp->GetUpVector();
		FVector f = CameraComp->GetForwardVector(); 
		
		FVector rx(r.X, u.X, f.X); 
		FVector ry(r.Y, u.Y, f.Y); 
		FVector rz(r.Z, u.Z, f.Z); 
		
		float z = f.Dot(P1 - pos); 
		FVector vpos(ScreenAvgPos.X * z, ScreenAvgPos.Y * z, z); 
		FVector AvgPos = FVector(vpos.Dot(rx), vpos.Dot(ry), vpos.Dot(rz)) + pos; 

		MaskComp->ClipPlaneNormal = -Dist.GetSafeNormal(); 
		BoundaryComp->ClipPlaneNormal = -Dist.GetSafeNormal(); 
		MaskComp->ClipPlaneBase = AvgPos + MaskComp->ClipPlaneNormal * 15.0f; 
		BoundaryComp->ClipPlaneBase = AvgPos - MaskComp->ClipPlaneNormal * 15.0f;
	} 
} 

void ACaptureCamera::FindPlayers()
{ 
	if (GM->Players.Num() != 2)
	{
		return; 
	}

	auto P1 = Cast<ASplitPlayer>(GM->Players[0]->GetPawn()); 
	auto P2 = Cast<ASplitPlayer>(GM->Players[1]->GetPawn());
	if (!IsValid(P1->ClonePlayer) || !IsValid(P2->ClonePlayer))
	{
		return; 
	}

	if (!CameraIdx)
	{
		Player1 = P1; 
		Player2 = P2->ClonePlayer; 
	}
	else 
	{
		Player1 = P1->ClonePlayer; 
		Player2 = P2; 
	}
	
	CalcPlayerScreenLocation(); 
} 

void ACaptureCamera::CalcPlayerScreenLocation()
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
		float z = f.Dot(pp - pos);
		float vx = r.Dot(pp - pos); 
		float vy = u.Dot(pp - pos); 
		px = vx / z;
		py = vy / z;
	}
	else 
	{ 
		FVector pp = Player2->GetActorLocation();
		float z = f.Dot(pp - pos);
		float vx = r.Dot(pp - pos);
		float vy = u.Dot(pp - pos);
		px = vx / z;
		py = vy / z; 
	}
	GM->PlayerScreenLocation[CameraIdx] = FVector2D(px, py);

	SetCameraLocation(); 
}

void ACaptureCamera::SetCameraLocation()
{ 
	FVector2D P1 = GM->PlayerScreenLocation[0]; 
	FVector2D P2 = GM->PlayerScreenLocation[1]; 
	ScreenAvgPos = (P1 - P2) * 0.5f + P2;

	switch (GM->CurPart)
	{
	case EMapPart::Part1: 
		float Min_X = FMath::Min(P1.X, P2.X); 
		if (Min_X < -0.5f) 
		{ 
			SetActorLocation(GetActorLocation() + FVector(0, -1, 0) * 1000.0f * GetWorld()->GetDeltaSeconds()); 
		} 
		else if (Min_X > -0.15f)
		{
			SetActorLocation(GetActorLocation() + FVector(0, 1, 0) * 1000.0f * GetWorld()->GetDeltaSeconds());
		}
		break;
	case EMapPart::Part2:
		float Min_Y = FMath::Min(P1.Y, P2.Y); 
		if (Min_Y < -0.4f) 
		{ 
			SetActorLocation(GetActorLocation() + FVector(0, -1, 0) * 1000.0f * GetWorld()->GetDeltaSeconds()); 
		} 
		else if (Min_Y > -0.2f)
		{
			SetActorLocation(GetActorLocation() + FVector(0, 1, 0) * 1000.0f * GetWorld()->GetDeltaSeconds());
		}
		break;
	case EMapPart::Part2_5: 
		break;
	case EMapPart::Part3: 
		break; 
	case EMapPart::Part4: 
		break; 
	}
}
