// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingObject.h"


// Sets default values
AMovingObject::AMovingObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsActive = false;
}

// Called when the game starts or when spawned
void AMovingObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bIsActive)
		return;

	// 좌 → 우 이동
	FVector Location = GetActorLocation();
	Location.X -= MoveSpeed * DeltaTime;
	SetActorLocation(Location);

	// 생존 시간 카운트
	LifeSpanRemaining -= DeltaTime;
	if (LifeSpanRemaining <= 0.f)
	{
		Deactivate(); // 풀로 반환 처리
	}
}

void AMovingObject::Activate()
{
	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	LifeSpanRemaining = MaxLifeSpan;

	// 콜리전도 다시 켜줘야 할 수 있음
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AMovingObject::Deactivate()
{
	bIsActive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	
	// 콜리전도 끔
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMovingObject::SetLifeSpanTime(float InTime)
{
	MaxLifeSpan = InTime;
}
void AMovingObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Execute(OtherActor);  // Trap 구조 사용
}


void AMovingObject::Execute(AActor* Interactor)
{
	/*if (Player->PlayerWorldType == EWorldType::SF)
	{
		// 1P면 죽게 처리
		Player->RespawnAtCheckpoint(); // 이건 캐릭터 클래스에 구현되어 있어야 함
	}
	else if (Player->PlayerWorldType == EWorldType::Fantasy)
	{
		// 2P면 무시, 그냥 밟고 지나가도록 (아무 일도 안 함)
	}*/
}