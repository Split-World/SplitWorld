// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SplitPlayerAnimInstance.h"
#include "SplitWorldGameModeBase.h" 
#include "GameFramework/Character.h" 
#include "SplitPlayer.generated.h"

struct FInputActionValue;

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* BodyComp;

	UPROPERTY(EditAnywhere)
	class UGroomComponent* HairComp;

	UPROPERTY(EditAnywhere)
	class UGroomComponent* EyebrowsComp;
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* IMC_Default;
	
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Jump;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Interact;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Dash;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Run;
	
	UFUNCTION()
	void MoveAction(const FInputActionValue& Value);

	UFUNCTION()
	void MoveCancle(const FInputActionValue& Value);

	UFUNCTION()
	void JumpAction(const FInputActionValue& Value);

	UFUNCTION()
	void InteractAction(const FInputActionValue& Value);

	UFUNCTION()
	void DashAction(const FInputActionValue& Value);

	UFUNCTION()
	void RunAction(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere)
	FTransform SpawnTransform;

	UFUNCTION()
	void Die();

	UFUNCTION(Server, Reliable)
	void MoveServer();

	UFUNCTION(NetMulticast, Reliable)
	void MoveMulti();
	
	UFUNCTION(Server, Reliable)
	void MoveCancleServer();

	UFUNCTION(NetMulticast, Reliable)
	void MoveCancleMulti();
	
	UPROPERTY(Replicated)
	bool bMoving = false;

	UFUNCTION(Server, Reliable)
	void JumpServer();

	UFUNCTION(NetMulticast, Reliable)
	void JumpMulti();
	
	UFUNCTION(NetMulticast, Reliable)
	void DoubleJumpMulti();
	
	UPROPERTY(Replicated)
	bool bJumping = false;
	UPROPERTY(Replicated)
	bool bDoubleJumping = false;
	float JumpSpeed = 0.0f;
	FVector JumpDir;
	FVector Dir;

	UFUNCTION(Server, Reliable)
	void DashServer();

	UFUNCTION(NetMulticast, Reliable)
	void DashMulti();
	
	UPROPERTY(Replicated)
	bool bDashing = false;

	UFUNCTION(Server, Reliable)
	void RunServer();

	UFUNCTION(NetMulticast, Reliable)
	void RunMulti(bool isRunning);
	
	UPROPERTY(Replicated)
	bool bRunning = false;

	UPROPERTY(Replicated)
	bool bClimbing = false;
	UPROPERTY(Replicated)
	bool bFailClimb = false;
	UPROPERTY(Replicated)
	bool bCanClimb = false;
	UPROPERTY(Replicated)
	bool bTraversal = false;
	UPROPERTY(Replicated)
	bool bAdjustAnimaition = false;
	
	bool DetectWall(FHitResult& Out_Hit, FVector& HitLocation, FVector& Normal, int& index);
	void ClimbWall(float Value);

	FVector MoveVectorUpward(FVector InVector, float AddValue);
	FVector MoveVectorDownward(FVector InVector, float SubtractValue);
	FVector MoveVectorForward(FVector InVector, FRotator InRotation, float AddValue);
	FVector MoveVectorBackward(FVector InVector, FRotator InRotation, float SubtractValue);
	FVector MoveVectorRightward(FVector InVector, FRotator InRotation, float AddValue);
	FVector MoveVectorLeftward(FVector InVector, FRotator InRotation, float SubtractValue);

	FRotator ReveseNormal(FVector InNormal);

	UPROPERTY(EditAnywhere)
	UAnimMontage* ClimbMontage;

	UPROPERTY()
	class USplitPlayerAnimInstance* anim;
	
	UPROPERTY(Replicated)
	class AClonePlayer* ClonePlayer;

	UFUNCTION(Server, Reliable)
	void SpawnClone(FVector PlayerStart, FVector LocationOffset);

	UPROPERTY(EditAnywhere)
	FVector Player1Start;
	UPROPERTY(EditAnywhere)
	FVector Player2Start;
	UPROPERTY(EditAnywhere)
	FVector CloneDist;

	UFUNCTION(Server, Reliable)

	void CloneLocation(FVector Location); 
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AClonePlayer> ClonePlayerFactory;

	UFUNCTION(Server, Reliable)
	void Interact(class AInteractableActorBase* Actor);

	UPROPERTY() 
	class ASplitWorldGameModeBase* GM; 

	UPROPERTY(EditAnywhere)
	TArray<FVector> Forwards; 
	UPROPERTY(EditAnywhere)
	TArray<FVector> Rights;

	UPROPERTY(Replicated)
	int CurPart = 0;

	void ChangePart(); 

	UPROPERTY(Replicated) 
	FVector4 MoveCheck; 

	void ConveyorBeltCheck(float DeltaTime);
};
