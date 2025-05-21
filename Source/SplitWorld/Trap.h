// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class SPLITWORLD_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Trap")
	bool bIsActive;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 플레이어와 상호작용 시 호출
	virtual void Execute(AActor* Interactor);

};
