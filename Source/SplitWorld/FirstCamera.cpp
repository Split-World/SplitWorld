// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstCamera.h" 

#include "ClonePlayer.h"
#include "FireLaser.h"
#include "SplitPlayer.h"
#include "SplitWorldGameModeBase.h" 
#include "Components/SceneCaptureComponent2D.h" 
#include "GameFramework/SpringArmComponent.h" 
#include "Net/UnrealNetwork.h" 
#include "SecondCamera.h" 
#include "Chaos/SoftsSpring.h"

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

	bReplicates = true;
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
} 

void AFirstCamera::BeginPlay()
{
	Super::BeginPlay(); 
	
	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode()); 
		GM->ChangePartDelegate.AddLambda([&](){ ChangePart(); }); 

		FTransform t = GetActorTransform(); 
		t.SetLocation(t.GetLocation() + LocationOffset); 
		SecondCamera = GetWorld()->SpawnActor<ASecondCamera>(SecondCameraFactory, t); 

		SetActorRotation(FRotator(-90.0f, 0.0f, 0.0f)); 
		ChangePart(); 
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
		}
		CameraTransformSync(); 
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
	DOREPLIFETIME(AFirstCamera, LastData);
	DOREPLIFETIME(AFirstCamera, CurSpringArmLength); 
}

FVector AFirstCamera::GetCameraLocation()
{
	return CameraComp->GetComponentLocation(); 
}

ASecondCamera* AFirstCamera::GetSecondCamera()
{
	return SecondCamera; 
}

void AFirstCamera::UpdateMask(float DeltaTime)
{
	FVector P1 = Player1->GetActorLocation();
	FVector P2 = Player2_Clone->GetActorLocation();
	FVector Dist = P1 - P2; 
	FVector AvgPos = Dist * 0.5f + P2; 

	FVector Normal = bIsLastPart ? GetNormal2D_YZ(-Dist) : -Dist.GetSafeNormal2D(); 
	MaskComp->ClipPlaneNormal = FMath::Lerp(MaskComp->ClipPlaneNormal, Normal, 6.0f * DeltaTime);
	BoundaryComp->ClipPlaneNormal = FMath::Lerp(BoundaryComp->ClipPlaneNormal, Normal, 6.0f * DeltaTime); 
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
	
	Player1->MoveCheck.X = int(px < 0.975f); 
	Player1->MoveCheck.Y = int(px > -0.975f); 
	Player1->MoveCheck.Z = int(py < 0.495f);  
	Player1->MoveCheck.W = int(py > -0.495f); 

	pp = Player2_Clone->GetActorLocation(); 
	z = f.Dot(pp - pos);
	vx = r.Dot(pp - pos);
	vy = u.Dot(pp - pos);
	px = vx / z;
	py = vy / z;
	PlayerScreenLocation[1] = FVector2D(px, py); 

	Player2->MoveCheck.X = int(px < 0.975f);
	Player2->MoveCheck.Y = int(px > -0.975f);
	Player2->MoveCheck.Z = int(py < 0.535f);
	Player2->MoveCheck.W = int(py > -0.535f);
} 

void AFirstCamera::SetCameraLocation(float DeltaTime)
{
	if (ViewChangePercent < 1.0f)
	{ 
		ViewChangePercent = FMath::Min(ViewChangePercent + DeltaTime / CameraDatas[int(GM->CurPart)].Speed, 1.0f); 
		SetActorLocation(FMath::Lerp(LastData.Location, CameraDatas[int(GM->CurPart)].Location, ViewChangePercent));
		SetActorRotation(FQuat::Slerp(LastData.Rotation.Quaternion(), CameraDatas[int(GM->CurPart)].Rotation.Quaternion(), ViewChangePercent));
		SpringArmComp->TargetArmLength = FMath::Lerp(LastData.Length, CameraDatas[int(GM->CurPart)].Length, ViewChangePercent); 
		CurSpringArmLength = SpringArmComp->TargetArmLength; 
		
		if (GM->CurPart == EMapPart::PartDoor)
		{
			GM->DoorGauge = 0.0f; 
		}
	}
	else if (GM->CurPart == EMapPart::PartDoor)
	{
		FRotator Rot = CameraDatas[int(GM->CurPart)].Rotation; 
		float Yaw = FMath::Lerp(0, 90.0f, GM->DoorGauge / 10.0f); 
		Rot.Yaw = Yaw;
		SetActorRotation(Rot); 
	} 
	else 
	{
		FVector2D P1 = PlayerScreenLocation[0];
		FVector2D P2 = PlayerScreenLocation[1];
		float Min_X = FMath::Min(P1.X, P2.X); 
		float Min_Y = FMath::Min(P1.Y, P2.Y); 

		switch (GM->CurPart)
		{
		case EMapPart::Part1:
			if (Min_X < -0.5f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(0, -100, 0), 12.0f * DeltaTime));
			}
			else if (Min_X > -0.15f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(0, 100, 0), 12.0f * DeltaTime));
			}
			break;
		case EMapPart::Part2:
		case EMapPart::Part2_5:
			if (Min_Y < -0.4f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(0, -100, 0), 12.0f * DeltaTime));
			}
			else if (Min_Y > -0.2f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(0, 100, 0), 12.0f * DeltaTime));
			}
			break;
		case EMapPart::Part3:
		case EMapPart::Part3_5:
			if (Min_X < -0.5f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(100, 0, 0), 12.0f * DeltaTime));
			}
			else if (Min_X > -0.15f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(-100, 0, 0), 12.0f * DeltaTime));
			}
			break;
		case EMapPart::Part4:
			if (Min_Y > 0.1f)
			{
				SetActorLocation(FMath::Lerp(GetActorLocation(), GetActorLocation() + FVector(0, 0, 100), 12.0f * DeltaTime));
			}
			break;
		}
	}
} 

void AFirstCamera::CameraTransformSync() 
{ 
	if (IsValid(SecondCamera))
	{
		FTransform t = GetActorTransform();
		t.SetLocation(t.GetLocation() + LocationOffset);
		SecondCamera->SetActorTransform(t);
		SpringArmComp->TargetArmLength = CurSpringArmLength;
		SecondCamera->GetSpringArm()->TargetArmLength = CurSpringArmLength;
	} 
}

void AFirstCamera::ChangePart()
{ 
	ViewChangePercent = 0.0f;
	if (GM->CurPart == EMapPart::Part4)
	{
		ChangeMask();
	}
	
	LastData.Location = GetActorLocation();
	LastData.Rotation = GetActorRotation();
	LastData.Length = SpringArmComp->TargetArmLength; 
}

FVector AFirstCamera::GetNormal2D_YZ(const FVector& origin)
{
	FVector2D t(origin.Y, origin.Z);
	t.Normalize();
	return FVector(0, t.X, t.Y); 
}

void AFirstCamera::ChangeMask_Implementation()
{
	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, [&]()
	{
		MaskComp->ShowOnlyActors.RemoveAt(0); 
		BoundaryComp->ShowOnlyActors.RemoveAt(0);
		MaskComp->ShowOnlyActors.Add(MaskFloors); 
		BoundaryComp->ShowOnlyActors.Add(MaskFloors);

		bIsLastPart = true; 
	}, 1.0f, false); 
}
