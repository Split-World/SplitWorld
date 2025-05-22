// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitPlayer.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASplitPlayer::ASplitPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh
	(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
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
}

// Called to bind functionality to input
void ASplitPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASplitPlayer::MoveAction);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
}

void ASplitPlayer::MoveAction(const FInputActionValue& Value)
{
	FRotator rot = GetControlRotation();
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(rot.Roll, 0,rot.Yaw)), Value.Get<FVector>().X, false);
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, 0,rot.Yaw)), Value.Get<FVector>().Y, false);
}

void ASplitPlayer::JumpAction(const FInputActionValue& Value)
{
	Jump();
}

