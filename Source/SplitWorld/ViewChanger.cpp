// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewChanger.h" 
#include "Components/BoxComponent.h" 

AViewChanger::AViewChanger()
{ 
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp")); 
	SetRootComponent(BoxComp); 
	BoxComp->SetBoxExtent(FVector(50.0f)); 
}

void AViewChanger::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AViewChanger::OnBoxBeginOverlap); 
}

void AViewChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AViewChanger::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{ 

}
