// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h" 
#include "Components/BoxComponent.h" 

ATrap::ATrap()
{ 
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp")); 
	SetRootComponent(BoxComp); 
}

void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

