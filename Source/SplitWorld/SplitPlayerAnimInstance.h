// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SplitPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPLITWORLD_API USplitPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USplitPlayerAnimInstance();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bRunning = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bJumping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bDoubleJumping = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bClimbing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bPush = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bPushing = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool bDashing = false;

	
};
