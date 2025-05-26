// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitPlayer.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Animation/AnimInstanceProxy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASplitPlayer::ASplitPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh
	(TEXT("/Script/Engine.SkeletalMesh'/Game/Assets/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}
	
	GetCharacterMovement()->JumpZVelocity = 500.f;

	ConstructorHelpers::FObjectFinder<UInputMappingContext>tempIMC
	(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/JS_Folder/Inputs/IMC_Default.IMC_Default'"));
	
	if (tempIMC.Succeeded())
	{
		IMC_Default = tempIMC.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction>tempMoveIA
	(TEXT("/Script/EnhancedInput.InputAction'/Game/JS_Folder/Inputs/InputActions/IA_Move.IA_Move'"));

	if (tempMoveIA.Succeeded())
	{
		IA_Move = tempMoveIA.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction>tempJumpIA
(TEXT("/Script/EnhancedInput.InputAction'/Game/JS_Folder/Inputs/InputActions/IA_Jump.IA_Jump'"));

	if (tempJumpIA.Succeeded())
	{
		IA_Jump = tempJumpIA.Object;
	}
}

// Called when the game starts or when spawned
void ASplitPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	JumpMaxCount = 2;
}

void ASplitPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Default, 0);
		}
	}
}

// Called every frame
void ASplitPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetCharacterMovement()->IsFalling())
	{
		bJumping = false;
		bDoubleJumping = false;
	}
}

// Called to bind functionality to input
void ASplitPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASplitPlayer::MoveAction);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ASplitPlayer::JumpAction);
	}
}

void ASplitPlayer::MoveAction(const FInputActionValue& Value)
{
	FRotator rot = GetControlRotation();
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(rot.Roll, 0,rot.Yaw)), Value.Get<FVector>().X, false);
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, 0,rot.Yaw)), Value.Get<FVector>().Y, false);


	FVector HitLocation;
	FVector Normal;
	int index;

	DetectWall(HitLocation,Normal, index);
}

void ASplitPlayer::JumpAction(const FInputActionValue& Value)
{
	if (bJumping)
	{
		Jump();
		bJumping = true;
	}
	else if (bDoubleJumping)
	{
		Jump();
		bDoubleJumping = true;
	}
}

void ASplitPlayer::Die()
{
	SetActorTransform(SpawnTransform);
}

bool ASplitPlayer::DetectWall(FVector& HitLocation, FVector& Normal, int& index)
{
	FHitResult OutHit;
	
	for (int detectIndex  = 0; detectIndex < 2; detectIndex++)
	{
		FVector tempLocation = MoveVectorUpward(MoveVectorUpward(GetActorLocation(), 45.f), detectIndex * 20.f);
		FVector StartVector = MoveVectorBackward(tempLocation, GetActorRotation(), 30.f);
		FVector EndVector = (GetActorForwardVector() * 100.f) + tempLocation;
		
		TArray<AActor*> ignoreActors;
		ignoreActors.Add(this);
		
		bool bHit = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			StartVector,
			EndVector,
			10.f,
			(ETraceTypeQuery)ECC_Visibility,
			false,
			ignoreActors,
			EDrawDebugTrace::ForOneFrame,
			OutHit,
			true,
			FColor::Yellow,
			FColor::Green,
			5.f
			);

		if (bHit)
		{
			index = detectIndex;
			break;
		}
	}

	HitLocation=OutHit.Location;
	Normal = OutHit.Normal;
	return OutHit.bBlockingHit;
}

FVector ASplitPlayer::MoveVectorUpward(FVector InVector, float AddValue)
{
	InVector.Z += AddValue;
	return InVector;
}

FVector ASplitPlayer::MoveVectorDownward(FVector InVector, float SubtractValue)
{
	InVector.Z -= SubtractValue;
	return InVector;
}

FVector ASplitPlayer::MoveVectorForward(FVector InVector, FRotator InRotation, float AddValue)
{
	InVector += InRotation.Vector() * AddValue;
	return InVector;
}

FVector ASplitPlayer::MoveVectorBackward(FVector InVector, FRotator InRotation, float SubtractValue)
{
	InVector -= InRotation.Vector() * SubtractValue;
	return InVector;
}

FVector ASplitPlayer::MoveVectorRightward(FVector InVector, FRotator InRotation, float AddValue)
{
	InVector += InRotation.RotateVector(GetActorRightVector()) * AddValue;
	return InVector;
}

FVector ASplitPlayer::MoveVectorLeftward(FVector InVector, FRotator InRotation, float SubtractValue)
{
	InVector -= InRotation.RotateVector(GetActorRightVector()) * SubtractValue;
	return InVector;
}

FRotator ASplitPlayer::ReveseNormal(FVector InNormal)
{
	return UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(InNormal),FRotator(0.f ,0.f ,180.f));
}



