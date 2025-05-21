// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.h"
#include "MovingObject.h"
#include "MovingObjectManager.generated.h"

UCLASS()
class SPLITWORLD_API AMovingObjectManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingObjectManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// Trap 기반 객체 관리
	UPROPERTY(EditAnywhere, Category = "Pool")
	TSubclassOf<ATrap> ObjectClass;

	// 풀 배열
	UPROPERTY()
	TArray<ATrap*> ObjectPool;

	// 초기 풀 크기
	UPROPERTY(EditAnywhere, Category = "Pool")
	int32 InitialPoolSize = 10;

	// 오브젝트 생존 시간
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float ObjectLifeSpan = 5.0f;

	// 스폰 시간 범위
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float MinSpawnInterval = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float MaxSpawnInterval = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float MapOffset = -10000.f;

	// 타이머 핸들
	FTimerHandle SpawnTimerHandle;


	
public:
	// 오브젝트 스폰 함수
	void SpawnObject();

	// 다음 스폰 예약
	void ScheduleNextSpawn();

	// 풀 초기화
	void InitializePool(int32 InitialSize);

	// 사용 가능한 오브젝트 요청
	ATrap* GetPooledObject();

	
};
