// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h" 

#include "Laser.h"
#include "SplitPlayer.h"
#include "Components/BoxComponent.h" 

ATrap::ATrap()
{ 
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp")); 
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50.0f)); 
	BoxComp->SetCollisionProfileName(TEXT("Trap")); 
	BoxComp->SetIsReplicated(true); 

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp")); 
	MeshComp->SetupAttachment(BoxComp); 
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));  
	MeshComp->SetIsReplicated(true); 
}

void ATrap::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnBoxBeginOverlap); 
}

void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrap::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<ASplitPlayer>(OtherActor); 

	if (Player)
	{
		Execute();
	} 
}
