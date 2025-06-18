// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadExtinction.h" 
#include "Components/BoxComponent.h" 
#include "SplitPlayer.h" 
#include "SplitWorldGameInstance.h" 
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h" 
#include "Materials/MaterialParameterCollectionInstance.h" 

ARoadExtinction::ARoadExtinction()
{ 
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); 
	SetRootComponent(Root); 

	StartBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("StartBoxComp")); 
	StartBoxComp->SetupAttachment(Root); 
	StartBoxComp->SetBoxExtent(FVector(50.0f)); 
	StartBoxComp->SetIsReplicated(true); 

	DestroyBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("DestroyBoxComp"));
	DestroyBoxComp->SetupAttachment(Root);
	DestroyBoxComp->SetBoxExtent(FVector(50.0f));
	DestroyBoxComp->SetIsReplicated(true); 
	
	bReplicates = true;
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
}

void ARoadExtinction::BeginPlay()
{
	Super::BeginPlay();
	
	StartBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARoadExtinction::OnStartBoxBeginOverlap); 
	DestroyBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARoadExtinction::OnDestroyBoxBeginOverlap); 

	MPC_Instance = GetWorld()->GetParameterCollectionInstance(MPC_SplitWorld); 
}

void ARoadExtinction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	if (HasAuthority() && bStart)
	{ 
		if (GetActorLocation().Z > 7100.0f)
		{
			if (Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode())->CurPart == EMapPart::Part4)
			{
				if (auto GI = Cast<USplitWorldGameInstance>(GetWorld()->GetGameInstance()))
				{
					GI->ExitRoom(); 
				}
			} 
		}
		else
		{
			SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, 1.0f) * 100.0f * DeltaTime); 
			Multi_ChangeWorld_Z(GetActorLocation().Z);
		} 
	} 
}

void ARoadExtinction::OnStartBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<ASplitPlayer>(OtherActor);
	if (Player)
	{
		bStart = true;
		StartBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UGameplayStatics::PlaySound2D(GetWorld(), StartSound); 
		UGameplayStatics::PlaySound2D(GetWorld(), ExtinctionSound); 
	} 
} 

void ARoadExtinction::OnDestroyBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{ 
	auto Player = Cast<ASplitPlayer>(OtherActor); 
	if (Player) 
	{ 
		Player->Die(); 
	} 
}

void ARoadExtinction::Multi_ChangeWorld_Z_Implementation(float World_Z)
{ 
	MPC_Instance->SetScalarParameterValue(FName(TEXT("World_Z")), World_Z); 
}
