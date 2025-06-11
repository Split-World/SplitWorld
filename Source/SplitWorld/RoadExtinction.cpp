// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadExtinction.h" 
#include "Components/BoxComponent.h" 
#include "SplitPlayer.h" 
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

	SetReplicates(true); 
	SetReplicateMovement(true); 
	bAlwaysRelevant = true; 
}

void ARoadExtinction::BeginPlay()
{
	Super::BeginPlay();
	
	StartBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARoadExtinction::OnStartBoxBeginOverlap); 
	DestroyBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARoadExtinction::OnDestroyBoxBeginOverlap); 
}

void ARoadExtinction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{ 
		SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, 1.0f) * 100.0f * DeltaTime); 
		if (auto MPC_Instance = GetWorld()->GetParameterCollectionInstance(MPC_Extinction))
		{
			MPC_Instance->SetScalarParameterValue(FName(TEXT("World_Z")), GetActorLocation().Z);
		}
	} 
}

void ARoadExtinction::OnStartBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{ 
	bStart = true; 
	StartBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}

void ARoadExtinction::OnDestroyBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{ 
	auto Player = Cast<ASplitPlayer>(OtherActor); 
	if (Player) 
	{ 
		Player->Die(); 
	} 
}
