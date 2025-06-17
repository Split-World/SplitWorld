// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "SplitPlayer.h"
#include "Components/SphereComponent.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASpawnPoint::OnBeginOverlap);
}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnPoint::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASplitPlayer *player = Cast<ASplitPlayer>(OtherActor);
	if (nullptr != player && player->CurPart != 6) player -> SpawnTransform = GetActorTransform();
}

