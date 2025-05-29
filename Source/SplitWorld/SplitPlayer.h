// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SplitPlayer.generated.h"

UCLASS()
class SPLITWORLD_API ASplitPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASplitPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void NotifyControllerChanged() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IMC_Default;
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Jump;
	
	UFUNCTION()
	void MoveAction(const FInputActionValue& Value);

	UFUNCTION()
	void JumpAction(const FInputActionValue& Value);

	FVector Direction;

	UPROPERTY(EditAnywhere)
	FTransform SpawnTransform;

	UFUNCTION()
	void Die();

	bool bJumping = false;
	bool bDoubleJumping = false;

	bool DetectWall(FVector& HitLocation, FVector& Normal, int& index);

	FVector MoveVectorUpward(FVector InVector, float AddValue);
	FVector MoveVectorDownward(FVector InVector, float SubtractValue);
	FVector MoveVectorForward(FVector InVector, FRotator InRotation, float AddValue);
	FVector MoveVectorBackward(FVector InVector, FRotator InRotation, float SubtractValue);
	FVector MoveVectorRightward(FVector InVector, FRotator InRotation, float AddValue);
	FVector MoveVectorLeftward(FVector InVector, FRotator InRotation, float SubtractValue);

	FRotator ReveseNormal(FVector InNormal); 
};
