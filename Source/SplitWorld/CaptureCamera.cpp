// Fill out your copyright notice in the Description page of Project Settings.


#include "CaptureCamera.h" 
#include "ClonePlayer.h"
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
	SpringArmComp->TargetArmLength = 500.0f; 

	CameraComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeRotation(FRotator(20.0f, 0.0f, 0.0f));

	bAlwaysRelevant = true; 
} 

void ACaptureCamera::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FRotator(-90.0f, 0.0f, 0.0f)); 
	
	GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode()); 
} 

void ACaptureCamera::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime); 
	
	if (IsValid(Player1) && IsValid(Player2))
	{
		UpdateMask(); 

		if (!CameraIdx)
		{ 
			float theta = CameraComp->FOVAngle / 2.0f;
			float d = 1.0f / FMath::Tan(FMath::DegreesToRadians(theta));
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("a : %f, %f"), theta, d)); 
			FVector pos = CameraComp->GetComponentLocation(); 
			FVector r = CameraComp->GetRightVector(); 
			FVector u = CameraComp->GetUpVector(); 
			FVector f = CameraComp->GetForwardVector();
			FVector pp = Player1->GetActorLocation(); 
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("b : %f, %f, %f"), pos.X, pos.Y, pos.Z)); 
			float z = f.Dot(Player1->GetActorLocation() - pos); 
			float vx = r.X * pp.X + r.Y * pp.Y + r.Z * pp.Z - r.Dot(pos); 
			float vy = u.X * pp.X + u.Y * pp.Y + u.Z * pp.Z - u.Dot(pos); 
			float px = d * vx / z; 
			float py = d * vy / z; 
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("c : %f, %f"), px, py)); 
		}
		else 
		{
			auto con2 = Cast<APlayerController>(Player2->Controller);

		} 
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
	DOREPLIFETIME(ACaptureCamera, CameraIdx); 
	DOREPLIFETIME(ACaptureCamera, Player1); 
	DOREPLIFETIME(ACaptureCamera, Player2); 
	DOREPLIFETIME(ACaptureCamera, Mask); 
}

void ACaptureCamera::UpdateMask()
{ 
	if (!CameraIdx)
	{
		FVector P1 = Player1->GetActorLocation();
		FVector P2 = Player2->GetActorLocation();
		FVector Dist = P1 - P2;
		FVector AvgPos = Dist * 0.5f + P2;

		auto Comp = Mask->GetCaptureComponent2D();
		Comp->ClipPlaneBase = AvgPos;

		if (Dist.Size2D() < 300.0f)
		{
			Comp->ClipPlaneNormal = -Dist.GetSafeNormal2D();
		}
		else
		{
			Comp->ClipPlaneNormal = -Dist.GetSafeNormal();
		}
	}
}

void ACaptureCamera::FindPlayers()
{ 
	if (!GM || GM->Players.Num() != 2)
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
