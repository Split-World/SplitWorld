// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish.h"

#include "Snake.h"
#include "SplitWorldGameModeBase.h"
#include "Net/UnrealNetwork.h"

AFish::AFish()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("Fish"));
	Mesh->SetIsReplicated(true);

	bReplicates = true;
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
}

void AFish::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AFish::OnMeshBeginOverlap); 
	
	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	StartLocation = GetActorLocation(); 
}

void AFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bLaunched)
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * 400.0f * DeltaTime); 
	} 
}

void AFish::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFish, bLaunched); 
}

void AFish::Launch()
{
	bLaunched = true;
	GM->bPlayer_Interactions[1] &= ~4;  
}

void AFish::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Snake = Cast<ASnake>(OtherActor))
	{
		UpdateSuccess(Snake->bActive); 
	} 
}

void AFish::UpdateSuccess_Implementation(bool bActive)
{
	if (bActive)
	{ 
		GM->bPlayer_Interactions[1] |= 2; 
	}
	else
	{
		SetActorLocation(StartLocation);
		GM->bPlayer_Interactions[1] |= 4;
	}
	
	bLaunched = false; 
}

