// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstCamera.h" 
#include "ClonePlayer.h" 
#include "SplitPlayer.h"
#include "SplitWorldGameModeBase.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Net/UnrealNetwork.h" 
#include "SecondCamera.h" 

AFirstCamera::AFirstCamera()
{
 	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SetRootComponent(SpringArmComp);
	SpringArmComp->TargetArmLength = 3000.0f; 
	SpringArmComp->SetIsReplicated(true); 

	CameraComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeRotation(FRotator(12.5f, 0.0f, 0.0f));
	CameraComp->SetIsReplicated(true); 

	MaskComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MaskComp"));
	MaskComp->SetupAttachment(SpringArmComp);
	MaskComp->SetRelativeRotation(FRotator(12.5f, 0.0f, 0.0f)); 
	MaskComp->SetIsReplicated(true); 
	MaskComp->bEnableClipPlane = true;

	BoundaryComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("BoundaryComp"));
	BoundaryComp->SetupAttachment(SpringArmComp);
	BoundaryComp->SetRelativeRotation(FRotator(12.5f, 0.0f, 0.0f));
	BoundaryComp->SetIsReplicated(true); 
	BoundaryComp->bEnableClipPlane = true; 

	SetReplicates(true); 
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
} 

void AFirstCamera::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FRotator(-90.0f, 0.0f, 0.0f));

	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode()); 
		GM->ChangePartDelegate.AddLambda([&](){ ChangePart(); }); 

		FTransform t = GetActorTransform(); 
		t.SetLocation(t.GetLocation() + LocationOffset); 
		SecondCamera = GetWorld()->SpawnActor<ASecondCamera>(SecondCameraFactory, t); 
	} 
} 

void AFirstCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	if (IsValid(Player1) && IsValid(Player2_Clone)) 
	{
		if (HasAuthority())
		{ 
			CalcPlayerScreenLocation();
			SetCameraLocation(DeltaTime); 
			CameraTransformSync(); 
		}
		UpdateMask(DeltaTime); 
	}
	else
	{
		if (HasAuthority())
		{
			FindPlayers();
		}
	}
} 

void AFirstCamera::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFirstCamera, SecondCamera); 
	DOREPLIFETIME(AFirstCamera, Player1); 
	DOREPLIFETIME(AFirstCamera, Player2); 
	DOREPLIFETIME(AFirstCamera, Player2_Clone); 
	DOREPLIFETIME(AFirstCamera, ScreenAvgPos); 
}

void AFirstCamera::UpdateMask(float DeltaTime)
{
	FVector P1 = Player1->GetActorLocation();
	FVector P2 = Player2_Clone->GetActorLocation();
	FVector Dist = P1 - P2; 
	FVector AvgPos = Dist * 0.5f + P2; 

	//FVector pos = CameraComp->GetComponentLocation();
	//FVector r = CameraComp->GetRightVector();
	//FVector u = CameraComp->GetUpVector();
	//FVector f = CameraComp->GetForwardVector();

	//FVector rx(r.X, u.X, f.X);
	//FVector ry(r.Y, u.Y, f.Y);
	//FVector rz(r.Z, u.Z, f.Z);

	//float z = f.Dot(P1 - pos);
	//FVector vpos(ScreenAvgPos.X * z, ScreenAvgPos.Y * z, z);
	//FVector AvgPos = FVector(vpos.Dot(rx), vpos.Dot(ry), vpos.Dot(rz)) + pos; 

	MaskComp->ClipPlaneNormal = FMath::Lerp(MaskComp->ClipPlaneNormal, -Dist.GetSafeNormal2D(), 6.0f * DeltaTime);
	BoundaryComp->ClipPlaneNormal = FMath::Lerp(BoundaryComp->ClipPlaneNormal, -Dist.GetSafeNormal2D(), 6.0f * DeltaTime); 
	MaskComp->ClipPlaneBase = FMath::Lerp(MaskComp->ClipPlaneBase, AvgPos + MaskComp->ClipPlaneNormal * 15.0f, 6.0f * DeltaTime);
	BoundaryComp->ClipPlaneBase = FMath::Lerp(BoundaryComp->ClipPlaneBase, AvgPos - MaskComp->ClipPlaneNormal * 15.0f, 6.0f * DeltaTime); 
} 

void AFirstCamera::FindPlayers() 
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
	
	Player1 = P1; 
	Player2 = P2; 
	Player2_Clone = P2->ClonePlayer; 
} 

void AFirstCamera::CalcPlayerScreenLocation()
{
	FVector pos = CameraComp->GetComponentLocation();
	FVector r = CameraComp->GetRightVector();
	FVector u = CameraComp->GetUpVector();
	FVector f = CameraComp->GetForwardVector();

	FVector pp = Player1->GetActorLocation();
	float z = f.Dot(pp - pos); 
	float vx = r.Dot(pp - pos); 
	float vy = u.Dot(pp - pos); 
	float px = vx / z; 
	float py = vy / z; 
	PlayerScreenLocation[0] = FVector2D(px, py); 
	
	Player1->MoveCheck.X = int(px < 0.9f); 
	Player1->MoveCheck.Y = int(px > -0.9f); 
	Player1->MoveCheck.Z = int(py < 0.45f);  
	Player1->MoveCheck.W = int(py > -0.45f); 

	pp = Player2_Clone->GetActorLocation(); 
	z = f.Dot(pp - pos);
	vx = r.Dot(pp - pos);
	vy = u.Dot(pp - pos);
	px = vx / z;
	py = vy / z;
	PlayerScreenLocation[1] = FVector2D(px, py); 

	Player2->MoveCheck.X = int(px < 0.9f);
	Player2->MoveCheck.Y = int(px > -0.9f);
	Player2->MoveCheck.Z = int(py < 0.45f);
	Player2->MoveCheck.W = int(py > -0.45f);
} 

void AFirstCamera::SetCameraLocation(float DeltaTime)
{ 
	if (ViewChangePercent < 1.0f)
	{ 
		ViewChangePercent = FMath::Min(ViewChangePercent + DeltaTime / 2.0f, 1.0f); 
		SetActorLocation(FMath::Lerp(GetActorLocation(), CameraDatas[int(GM->CurPart)].Location, ViewChangePercent));
		SetActorRotation(FQuat::Slerp(GetActorRotation().Quaternion(), CameraDatas[int(GM->CurPart)].Rotation.Quaternion(), ViewChangePercent));
		SpringArmComp->TargetArmLength = FMath::Lerp(SpringArmComp->TargetArmLength, CameraDatas[int(GM->CurPart)].Length, ViewChangePercent);
	} 
	else
	{ 
		FVector2D P1 = PlayerScreenLocation[0];
		FVector2D P2 = PlayerScreenLocation[1];
		ScreenAvgPos = (P1 - P2) * 0.5f + P2; 
		float Min_X = FMath::Min(P1.X, P2.X); 
		float Min_Y = FMath::Min(P1.Y, P2.Y); 

		switch (GM->CurPart)
		{
		case EMapPart::Part1:
			if (Min_X < -0.5f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(0, -50, 0), 24.0f * DeltaTime));
			}
			else if (Min_X > -0.15f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(0, 50, 0), 24.0f * DeltaTime));
			}
			break;
		case EMapPart::Part2:
		case EMapPart::Part2_5:
			if (Min_Y < -0.4f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(0, -50, 0), 24.0f * DeltaTime));
			}
			else if (Min_Y > -0.2f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(0, 50, 0), 24.0f * DeltaTime));
			}
			break;
		case EMapPart::Part3:
		case EMapPart::Part3_5:
			if (Min_X < -0.5f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(50, 0, 0), 48.0f * DeltaTime));
			}
			else if (Min_X > -0.15f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(-50, 0, 0), 48.0f * DeltaTime));
			}
			break;
		case EMapPart::Part4:
			if (Min_Y > 0.1f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(0, 0, 50), 48.0f * DeltaTime));
			}
			break;
		} 
	} 
} 

void AFirstCamera::CameraTransformSync() 
{ 
	FTransform t = GetActorTransform(); 
	t.SetLocation(t.GetLocation() + LocationOffset); 
	SecondCamera->SetActorTransform(t); 
}

void AFirstCamera::ChangePart()
{ 
	ViewChangePercent = 0.0f; 

}
