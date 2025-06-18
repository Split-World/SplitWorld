// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitPlayer.h"
#include "FishHandle.h" 
#include "ClonePlayer.h"
#include "Crack.h"
#include "DoorHandle.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FloorHandle.h"
#include "InputActionValue.h"
#include "Interactable.h"
#include "InteractableActorBase.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "SplitWorldGameModeBase.h" 
#include "SnakeHandle.h"
#include "SpawnPoint.h" 
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Sets default values
ASplitPlayer::ASplitPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMeshComp
	(TEXT("/Script/Engine.SkeletalMesh'/Game/Slay/Assets/Chr/Echo/Meshes/SK_Echo.SK_Echo'"));

	if (tempMeshComp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMeshComp.Object);
	}
	
	GetCharacterMovement()->JumpZVelocity = 500.f;

	ConstructorHelpers::FObjectFinder<UInputMappingContext>tempIMC
	(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/JS_Folder/Inputs/IMC_Default.IMC_Default'"));
	
	if (tempIMC.Succeeded())
	{
		IMC_Default = tempIMC.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction>tempIAMove
	(TEXT("/Script/EnhancedInput.InputAction'/Game/JS_Folder/Inputs/InputActions/IA_Move.IA_Move'"));

	if (tempIAMove.Succeeded())
	{
		IA_Move = tempIAMove.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction>tempIAJump
	(TEXT("/Script/EnhancedInput.InputAction'/Game/JS_Folder/Inputs/InputActions/IA_Jump.IA_Jump'"));

	if (tempIAJump.Succeeded())
	{
		IA_Jump = tempIAJump.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction>tempIAInteract
	(TEXT("/Script/EnhancedInput.InputAction'/Game/JS_Folder/Inputs/InputActions/IA_Interact.IA_Interact'"));

	if (tempIAInteract.Succeeded())
	{
		IA_Interact = tempIAInteract.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction>tempIADash
	(TEXT("/Script/EnhancedInput.InputAction'/Game/JS_Folder/Inputs/InputActions/IA_Dash.IA_Dash'"));

	if (tempIADash.Succeeded())
	{
		IA_Dash = tempIADash.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction>tempIARun
	(TEXT("/Script/EnhancedInput.InputAction'/Game/JS_Folder/Inputs/InputActions/IA_Run.IA_Run'"));

	if (tempIARun.Succeeded())
	{
		IA_Run = tempIARun.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UAnimMontage>tempTraversalMontage
	(TEXT("/Script/Engine.AnimMontage'/Game/JS_Folder/Animations/AM/AM_Traversal.AM_Traversal'"));
	
	if (tempTraversalMontage.Succeeded())
	{
		TraversalMontage = tempTraversalMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempRollMontage
	(TEXT("/Script/Engine.AnimMontage'/Game/JS_Folder/Animations/AM/AM_Roll.AM_Roll'"));
	
	if (tempRollMontage.Succeeded())
	{
		RollMontage = tempRollMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage>tempControlInteractMontage
	(TEXT("/Script/Engine.AnimMontage'/Game/JS_Folder/Animations/AM/AM_ControlInteract.AM_ControlInteract'"));
	
	if (tempControlInteractMontage.Succeeded())
	{
		ControlInteractMontage = tempControlInteractMontage.Object;
	}
	
	bAlwaysRelevant = true;

	DashVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashVFX"));
	DashVFX->SetupAttachment(GetMesh());

	DoubleJumpVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DoubleJumpVFX"));
	DoubleJumpVFX->SetupAttachment(GetMesh());

	DieVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DieVFX"));
	DieVFX->SetupAttachment(GetMesh());

	RespawnVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("RespawnVFX"));
	RespawnVFX->SetupAttachment(GetMesh());

	BG_Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("BG_Sound"));
	BG_Sound->SetupAttachment(GetMesh());

	Door_Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("Door_Sound"));
	Door_Sound->SetupAttachment(GetMesh());

	Water_Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("Water_Sound"));
	Water_Sound->SetupAttachment(GetMesh());

	Extinction_Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("Extinction_Sound"));
	Extinction_Sound->SetupAttachment(GetMesh()); 
}

// Called when the game starts or when spawned
void ASplitPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	JumpMaxCount = 2;

	if (IsLocallyControlled())
	{
		SpawnClone(HasAuthority() ? Player1Start : Player2Start, HasAuthority() ? CloneDist : -CloneDist);
	}

	anim = Cast<USplitPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
		GM->ChangePartDelegate.AddLambda([&](){ ChangePart(); }); 
	}

	MPC_Instance = GetWorld()->GetParameterCollectionInstance(collection);

	DashVFX->Deactivate();
	DoubleJumpVFX->Deactivate();
	DieVFX->Deactivate();
	RespawnVFX->Deactivate();

	PlaySound(BG_Sound);

	StopSound(Door_Sound);
	StopSound(Water_Sound);
	StopSound(Extinction_Sound);
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
	
	if (!IsValid(ClonePlayer)) return;
	if (!IsValid(anim)) return;
		
	if (IsLocallyControlled())
	{
		FTransform t = GetActorTransform();
		t.SetLocation((HasAuthority() ? CloneDist : -CloneDist) + GetActorLocation());
		CloneLocation(t);
	}
	
	if (bPush) PushingServer(!HasAuthority()); 
	
	if (!HasAuthority()) return; 

	ConveyorBeltCheck(DeltaTime);
	
	if (!GetCharacterMovement()->IsFalling())
	{
		if (OnGround) return;
		
		bJumping = false;
		bDoubleJumping = false;
		bFailClimb = false;
		bDashing = false;
		OnGround = true;
		
		OnGroundMulti();
	}
	else
	{
		FHitResult OutHit;
		FVector HitLocation;
		FVector Normal;
		int index;

		OnGround = false;
		
		if ((bMoving && !bFailClimb && !bTraversal && !bDashing && !bRolling) || bClimbing) bCanClimb = DetectWall(OutHit, HitLocation, Normal, index);
		
		if (bCanClimb)
		{
			if (index == 0)
			{
				float value = GetActorForwardVector().Dot(-OutHit.ImpactNormal);
				ClimbWall(FMath::RadiansToDegrees(FMath::Acos(value)));
			}
			else if (index == 1)
			{
				bTraversal = true;
				bClimbing = false; 
			
				TraversalMulti();
			}
		}
	}
	
	if (bAdjustAnimaition && bTraversal)
	{
		SetActorLocation(GetActorLocation() + GetActorUpVector() * GetWorld()->GetDeltaSeconds() * 200.f);
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * GetWorld()->GetDeltaSeconds() * 100.f); 
	} 
 
	if (bDashing)
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * GetWorld()->GetDeltaSeconds() * 600.f);
	}

	if (GM->DoorInput)
	{
		PlaySound(Door_Sound); 
	}
	else
	{
		StopSound(Door_Sound); 
	}
}

void ASplitPlayer::OnGroundMulti_Implementation()
{
	if (IsValid(anim) && IsValid(ClonePlayer))
	{
		anim->bJumping = false;
		anim->bDoubleJumping = false;
		anim->bDashing = false;
	
		ClonePlayer->anim->bJumping = false;
		ClonePlayer->anim->bDoubleJumping = false;
		ClonePlayer->anim->bDashing = false;
	}
}

void ASplitPlayer::TraversalMulti_Implementation()
{
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	
	anim->bClimbing = false;
	anim->Montage_Play(TraversalMontage);

	ClonePlayer->anim->bClimbing = false;
	ClonePlayer->anim->Montage_Play(TraversalMontage);

	GetCharacterMovement()->GravityScale = 0.0f;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASplitPlayer::StartTraversalServer_Implementation()
{
	bAdjustAnimaition = true;
	
	StartTraversalMulti();
}

void ASplitPlayer::StartTraversalMulti_Implementation()
{
	
}

void ASplitPlayer::EndTraversalServer_Implementation()
{
	bAdjustAnimaition = false;
	
	EndTraversalMulti();
}

void ASplitPlayer::EndTraversalMulti_Implementation()
{
	
}

void ASplitPlayer::EndClimbServer_Implementation()
{
	bTraversal = false;

	EndClimbMulti();
}

void ASplitPlayer::EndClimbMulti_Implementation()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->Velocity = FVector::ZeroVector; 
}

// Called to bind functionality to input
void ASplitPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASplitPlayer::MoveAction);
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &ASplitPlayer::MoveCancle);

		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ASplitPlayer::JumpAction);
		
		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &ASplitPlayer::InteractAction);
		
		EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Started, this, &ASplitPlayer::DashAction);
		
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &ASplitPlayer::RunAction);
	}
}

void ASplitPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASplitPlayer, bMoving);
	
	DOREPLIFETIME(ASplitPlayer, bJumping);
	DOREPLIFETIME(ASplitPlayer, bDoubleJumping);
	
	DOREPLIFETIME(ASplitPlayer, bClimbing);
	DOREPLIFETIME(ASplitPlayer, bFailClimb);
	DOREPLIFETIME(ASplitPlayer, bCanClimb);
	DOREPLIFETIME(ASplitPlayer, bTraversal);
	DOREPLIFETIME(ASplitPlayer, bAdjustAnimaition);

	DOREPLIFETIME(ASplitPlayer, bPush);
	DOREPLIFETIME(ASplitPlayer, bPushing);
	
	DOREPLIFETIME(ASplitPlayer, bDashing);
	DOREPLIFETIME(ASplitPlayer, bRolling);
	
	DOREPLIFETIME(ASplitPlayer, bRunning);
	
	DOREPLIFETIME(ASplitPlayer, ClonePlayer); 

	DOREPLIFETIME(ASplitPlayer, CurPart); 
	DOREPLIFETIME(ASplitPlayer, MoveCheck); 
}

void ASplitPlayer::MoveAction(const FInputActionValue& Value)
{
	if (bClimbing || bDashing || bTraversal || bRolling) return;
	
	if (!bMoving) MoveServer();

	FVector2D v = Value.Get<FVector2D>();
	Dir += Forwards[CurPart] * v.X * (v.X > 0 ? MoveCheck.Z : MoveCheck.W) * 60.0f * GetWorld()->GetDeltaSeconds(); 
	Dir += Rights[CurPart] * v.Y * (v.Y > 0 ? MoveCheck.X : MoveCheck.Y) * 60.0f * GetWorld()->GetDeltaSeconds();
	
	FRotator rot = GetControlRotation();
	if (GetCharacterMovement()->IsFalling())
	{
		AddMovementInput(Dir);
	}
	else 
	{
		AddMovementInput(Dir);
	}

	Dir = FVector(0); 
}

void ASplitPlayer::MoveServer_Implementation()
{
	bMoving = true;
	MoveMulti();
}

void ASplitPlayer::MoveMulti_Implementation()
{
	anim->bMoving = true;
	
	ClonePlayer->anim->bMoving = true;
}

void ASplitPlayer::MoveSoundServer_Implementation()
{
	MoveSoundMulti();
}

void ASplitPlayer::MoveSoundMulti_Implementation()
{
	UGameplayStatics::PlaySound2D(GetWorld(), WalkSound);
}

void ASplitPlayer::MoveCancle(const FInputActionValue& Value)
{
	MoveCancleServer();
}

void ASplitPlayer::MoveCancleServer_Implementation()
{
	bMoving = false;
	MoveCancleMulti();

	bRunning = false;
	RunMulti(false);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ASplitPlayer::MoveCancleMulti_Implementation()
{
	anim->bMoving = false;

	ClonePlayer->anim->bMoving = false;
}

void ASplitPlayer::JumpAction(const FInputActionValue& Value)
{
	if ((!bJumping || !bDoubleJumping) && !bDashing && !bClimbing && !bPush && !bTraversal && !bRolling)
	{
		Jump();
		JumpServer();
	}
}

void ASplitPlayer::JumpServer_Implementation()
{
	//GetCharacterMovement()->SetBase(nullptr);
	if (!bJumping)
	{
		bJumping = true;
		JumpMulti();
	}
	else if (!bDoubleJumping)
	{
		bDoubleJumping = true;
		DoubleJumpMulti();
	}
}

void ASplitPlayer::JumpMulti_Implementation()
{
	anim->bJumping = true;

	ClonePlayer->anim->bJumping = true;

	UGameplayStatics::PlaySound2D(GetWorld(), HA1Sound);
}

void ASplitPlayer::DoubleJumpMulti_Implementation()
{
	anim->bDoubleJumping = true;

	ClonePlayer->anim->bDoubleJumping = true;

	UGameplayStatics::PlaySound2D(GetWorld(), HA2Sound);
	
	DoubleJumpVFX->Activate();

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, [&]()
	{
		DoubleJumpVFX->Deactivate(); 
	}, 0.5f, false);  
}

void ASplitPlayer::InteractAction(const FInputActionValue& Value)
{
	FHitResult OutHit;
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);
	
	bool bHit = UKismetSystemLibrary::BoxTraceSingle(
	GetWorld(),
	GetActorLocation() + GetActorForwardVector() * 50.f,
	GetActorLocation() + GetActorForwardVector() * 50.f,
	FVector(25.f, 30.f, 70.f),
	GetActorRotation(),
	ETraceTypeQuery::TraceTypeQuery1,
	false,
	ignoreActors,
	EDrawDebugTrace::ForOneFrame,
	OutHit,
	true,
	FColor::Black,
	FColor::White,
	5.f
	);

	if (bHit)
	{
		auto II = Cast<AInteractableActorBase>(OutHit.GetActor()); 
		if (II && ((!II->Idx && HasAuthority()) || (II->Idx && !HasAuthority())))
		{
			InteractServer(II);
		}
	}
}

void ASplitPlayer::InteractServer_Implementation(AInteractableActorBase* Actor)
{
	IInteractable::Execute_Interaction(Actor);

	if (Actor->IsA<ADoorHandle>() && CurPart == int(EMapPart::Part1) || Actor->IsA<ACrack>() && CurPart == int(EMapPart::Part4))
	{
		bPush = true;

		InteractMulti();
	}
	
	if (Actor->IsA<AFishHandle>() || Actor->IsA<AFloorHandle>() || Actor->IsA<ASnakeHandle>())
	{
		ControlMulti();
	}
} 

void ASplitPlayer::InteractMulti_Implementation()
{
	anim->bPush = true;

	ClonePlayer->anim->bPush = true;
}

void ASplitPlayer::PushMulti()
{
	anim->bPushing = bPushing;

	ClonePlayer->anim->bPushing = bPushing; 
}

void ASplitPlayer::PushingServer_Implementation(bool bIsClient)
{
	bPushing = GM->DoorInput & 1 << (int)bIsClient; 
}

void ASplitPlayer::ControlMulti_Implementation()
{
	anim->Montage_Play(ControlInteractMontage);
		
	ClonePlayer->anim->Montage_Play(ControlInteractMontage);
}

void ASplitPlayer::DashAction(const FInputActionValue& Value)
{
	if (bRunning || bDashing || bClimbing || bPush || bRolling) return;
	
	if (!GetCharacterMovement()->IsFalling())
	{
		RollServer();
	}
	else
	{
		DashServer();
	}
}

void ASplitPlayer::RollServer_Implementation()
{
	bRolling = true;
	RollMulti();
}

void ASplitPlayer::RollMulti_Implementation()
{
	anim->Montage_Play(RollMontage);
		
	ClonePlayer->anim->Montage_Play(RollMontage);

	UGameplayStatics::PlaySound2D(GetWorld(), HA3Sound);
	UGameplayStatics::PlaySound2D(GetWorld(), DodgeSound);
}

void ASplitPlayer::EndRollServer_Implementation()
{
	bRolling = false;
	EndRollMulti(); 
}

void ASplitPlayer::EndRollMulti_Implementation()
{
	 
}

void ASplitPlayer::DashServer_Implementation()
{
	bDashing = true;
	DashMulti();
}

void ASplitPlayer::DashMulti_Implementation()
{
	anim->bDashing = true;

	ClonePlayer->anim->bDashing = true;

	UGameplayStatics::PlaySound2D(GetWorld(), HA4Sound);
	UGameplayStatics::PlaySound2D(GetWorld(), DashSound);

	DashVFX->Activate();

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, [&]()
	{
		DashVFX->Deactivate(); 
	}, 0.5f, false);  
}

void ASplitPlayer::RunAction(const FInputActionValue& Value)
{
	RunServer();
}

void ASplitPlayer::RunServer_Implementation()
{
	if (!bRunning && !bDashing && !bClimbing && !bJumping && bMoving && !bPush && !bTraversal && !bRolling)
	{
		bRunning = true;
		RunMulti(true);
		GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	}
	else
	{
		bRunning = false;
		RunMulti(false);
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void ASplitPlayer::RunMulti_Implementation(bool isRunning)
{
	anim->bRunning = isRunning;

	ClonePlayer->anim->bRunning = isRunning;
}

void ASplitPlayer::Die()
{
	if (CurPart == 6)
	{
		TArray<class AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), FoundActors);
		
		float Z;
		MPC_Instance->GetScalarParameterValue(FName(TEXT("World_Z")),Z);
		
		float distance = 999999.f;
		for (auto Actor : FoundActors)
		{ 
			if (Actor->GetActorLocation().Z > Z + 300.0f)
				if (distance > FVector::Dist(GetActorLocation(), Actor->GetActorLocation()))
				{
					distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
					SpawnTransform = Actor->GetActorTransform();
				}
		}
	}
	
	DieServer(SpawnTransform);
}

void ASplitPlayer::DieMulti_Implementation(FTransform NewTransform)
{
	UGameplayStatics::PlaySound2D(GetWorld(), DieSound);

	DieVFX->Activate(); 

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, [&]()
	{
		DieVFX->Deactivate();
		SetActorTransform(SpawnTransform);
		RespawnServer(); 
	}, 0.5f, false);
}

void ASplitPlayer::DieServer_Implementation(FTransform NewTransform)
{
	DieMulti(NewTransform); 
}

void ASplitPlayer::RespawnServer_Implementation()
{
	RespawnMulti(); 
}

void ASplitPlayer::RespawnMulti_Implementation()
{
	RespawnVFX->Activate(); 

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, [&]()
	{
		RespawnVFX->Deactivate(); 
	}, 0.5f, false); 
} 

bool ASplitPlayer::DetectWall(FHitResult& Out_Hit, FVector& HitLocation, FVector& Normal, int& index)	
{
	FHitResult OutHit;
	
	for (int detectIndex  = 0; detectIndex < 2; detectIndex++)
	{
		FVector tempLocation = MoveVectorDownward(MoveVectorUpward(GetActorLocation(), 30.f), detectIndex * 40.f);
		FVector StartVector = MoveVectorBackward(tempLocation, GetActorRotation(), 30.f);
		FVector EndVector = (GetActorForwardVector() * 50.f) + tempLocation;
		
		TArray<AActor*> ignoreActors;
		ignoreActors.Add(this); 
		
		bool bHit = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			StartVector,
			EndVector,
			10.f,
			ETraceTypeQuery::TraceTypeQuery1,
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

	Out_Hit = OutHit;
	HitLocation=OutHit.Location;
	Normal = OutHit.Normal;
	return OutHit.bBlockingHit;
}

void ASplitPlayer::ClimbWall(float Value)
{
	if (Value > 5.0f) return;

	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	
	if (!bClimbing) 
	{ 
		GetWorldTimerManager().ClearTimer(ClimbTimerHandle); 
		GetWorldTimerManager().SetTimer(ClimbTimerHandle, [&]()
		{
			bFailClimb = true;
			bClimbing = false;

			FailClimbMulti();
		}, 1.0f, false); 
		ClimbMulti();
	}

	bClimbing = true;

	FHitResult OutHit;
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
	GetWorld(),
	MoveVectorUpward(GetActorLocation(), 70.f),
	MoveVectorUpward(GetActorLocation(), 90.f),
	10.f,
	ETraceTypeQuery::TraceTypeQuery1,
	false,
	ignoreActors,
	EDrawDebugTrace::ForOneFrame,
	OutHit,
	true,
	FColor::Red,
	FColor::Blue,
	5.f
	); 

	if (!bHit) 
	{
		SetActorLocation(GetActorLocation() + GetActorUpVector() * 700.f * GetWorld()->GetDeltaSeconds());
	}
	else if (bHit)
	{
		bFailClimb = true;
		bClimbing = false;

		FailClimbMulti();
	}
}

void ASplitPlayer::ClimbMulti_Implementation()
{
	anim->bClimbing = true;
	
	ClonePlayer->anim->bClimbing = true;
}

void ASplitPlayer::FailClimbMulti_Implementation()
{
	anim->bClimbing = false;

	ClonePlayer->anim->bClimbing = false; 

	GetCharacterMovement()->GravityScale = 1.f; 
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

void ASplitPlayer::CloneLocation_Implementation(FTransform Transform)
{
	Transform.SetLocation(Transform.GetLocation() - FVector(0.f, 0.f, 90.f));

	FRotator curRot = Transform.GetRotation().Rotator();
	curRot.Yaw -= 90.f;
	Transform.SetRotation(curRot.Quaternion());
	
	ClonePlayer->SetActorTransform(Transform);
}

void ASplitPlayer::SpawnClone_Implementation(FVector PlayerStart, FVector LocationOffset)
{
	FTransform CloneSpawnTransform = GetActorTransform();
	CloneSpawnTransform.SetLocation(PlayerStart + LocationOffset);
	
	SetActorLocation(PlayerStart);

	ClonePlayer = GetWorld()-> SpawnActor<AClonePlayer>(ClonePlayerFactory, CloneSpawnTransform);
}

void ASplitPlayer::ChangePart()
{ 
	CurPart = int(GM->CurPart);

	if (CurPart == 2) CanclePushServer(); 

	if (CurPart == int(EMapPart::Part3))
	{
		PlaySound(Water_Sound);
	}
	else if (!(CurPart == int(EMapPart::Part3_5)))
	{
		StopSound(Water_Sound); 
	}

	if (CurPart == int(EMapPart::Part4))
	{
		PlaySound(Extinction_Sound); 
	}
	else
	{
		StopSound(Extinction_Sound); 
	}
} 

void ASplitPlayer::CanclePushServer_Implementation()
{
	bPush = false;

	ChangePartMulti();
}

void ASplitPlayer::ChangePartMulti_Implementation()
{
	anim->bPush = false;

	ClonePlayer->anim->bPush = false;
}

void ASplitPlayer::ConveyorBeltCheck(float DeltaTime)
{ 
	FHitResult Hit; 
	FVector Start = GetActorLocation(); 
	FVector End = Start + FVector(0, 0, -(GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 10)); 
	FCollisionQueryParams CQP; 
	CQP.AddIgnoredActor(this); 

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel4, CQP)) 
	{ 
		SetActorLocation(GetActorLocation() + FVector(0, -300.0f, 0) * DeltaTime); 
	}
}

void ASplitPlayer::StopSound_Implementation(UAudioComponent* Audio)
{
	Audio->Stop(); 
}

void ASplitPlayer::PlaySound_Implementation(UAudioComponent* Audio)
{ 
	Audio->Play(); 
}

