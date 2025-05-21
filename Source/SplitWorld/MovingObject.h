// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trap.h"
#include "GameFramework/Actor.h"
#include "MovingObject.generated.h"

UCLASS()
class SPLITWORLD_API AMovingObject : public ATrap
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovingObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
public:
	// 이동 속도
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 300.0f;

	// 현재 남은 생존 시간
	float LifeSpanRemaining;

	// 생존 시간 타이머
	float MaxLifeSpan;

	// 활성 상태인지 여부
	bool bIsActive;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Execute(AActor* Interactor) override;
	
	// 오브젝트 초기화 (스폰될 때 호출)
	void Activate();

	// 오브젝트 비활성화 (풀에 반환될 때)
	void Deactivate();

	// 비활성화까지의 생존 시간 설정
	void SetLifeSpanTime(float InTime);
};
