// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitPlayer.h"

#include "ClonePlayer.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Interactable.h"
#include "InteractableActorBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "SplitWorldGameModeBase.h" 
#include "GroomComponent.h" 
#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"

// Sets default values
ASplitPlayer::ASplitPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;

	BodyComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	BodyComp->SetupAttachment(GetMesh());

	BodyComp->SetRelativeLocation(FVector(0, 0, -90));
	BodyComp->SetRelativeRotation(FRotator(0, -90, 0));
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempBodyComp
	(TEXT("/Script/Engine.SkeletalMesh'/Game/Slay/Assets/Chr/Echo/Meshes/SK_Echo.SK_Echo'"));

	if (tempBodyComp.Succeeded())
	{
		BodyComp->SetSkeletalMesh(tempBodyComp.Object);
	}
	
	/*HairComp = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	HairComp->SetupAttachment(BodyComp);

	ConstructorHelpers::FObjectFinder<UGroomAsset>tempHairGroomAsset
	(TEXT("/Script/HairStrandsCore.GroomAsset'/Game/Slay/Assets/Chr/Echo/Hair/Hair_S_UpdoBuns.Hair_S_UpdoBuns'"));

	ConstructorHelpers::FObjectFinder<UGroomBindingAsset>tempHairBindingAsset
	(TEXT("/Script/HairStrandsCore.GroomBindingAsset'/Game/Slay/Assets/Chr/Echo/Hair/Hair_S_UpdoBuns_Echo_M3D_LOD0_Binding.Hair_S_UpdoBuns_Echo_M3D_LOD0_Binding'"));

	HairComp->SetGroomAsset(tempHairGroomAsset.Object);
	HairComp->SetBindingAsset(tempHairBindingAsset.Object);
	
	EyebrowsComp = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	EyebrowsComp->SetupAttachment(BodyComp);

	ConstructorHelpers::FObjectFinder<UGroomAsset>tempEyebrowsGroomAsset
	(TEXT("/Script/HairStrandsCore.GroomAsset'/Game/Slay/Assets/Chr/Echo/Hair/Eyebrows_L_Echo.Eyebrows_L_Echo'"));

	ConstructorHelpers::FObjectFinder<UGroomBindingAsset>tempEyebrowsBindingAsset
	(TEXT("/Script/HairStrandsCore.GroomBindingAsset'/Game/Slay/Assets/Chr/Echo/Hair/Eyebrows_L_Echo_Echo_M3D_LOD0_Binding.Eyebrows_L_Echo_Echo_M3D_LOD0_Binding'"));
	
	EyebrowsComp->SetGroomAsset(tempEyebrowsGroomAsset.Object);
	EyebrowsComp->SetBindingAsset(tempEyebrowsBindingAsset.Object);*/
	
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
	
	ConstructorHelpers::FObjectFinder<UAnimMontage>tempClimbMontage
	(TEXT("/Script/Engine.AnimMontage'/Game/JS_Folder/Animations/AM/AM_Climb.AM_Climb'"));
	
	if (tempClimbMontage.Succeeded())
	{
		ClimbMontage = tempClimbMontage.Object;
	}
	
	bAlwaysRelevant = true;
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

	anim = Cast<USplitPlayerAnimInstance>(BodyComp->GetAnimInstance());
	
	if (HasAuthority())
	{
		GM = Cast<ASplitWorldGameModeBase>(GetWorld()->GetAuthGameMode());
		GM->ChangePartDelegate.AddLambda([&](){ ChangePart(); }); 
	}
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
	if (!ClonePlayer) return;
	if (IsLocallyControlled())
	{ 
		CloneLocation((HasAuthority() ? CloneDist : -CloneDist) + GetActorLocation());
	}

	if (!HasAuthority())
	{ 
		return; 
	}

	if (!GetCharacterMovement()->IsFalling())
	{
		bJumping = false;
		bDoubleJumping = false;
		bFailClimb = false;
		bClimbing = false;
		bTraversal = false;
		bDashing = false;

		anim->bJumping = false;
		anim->bDoubleJumping = false;
		anim->bClimbing = false;
		anim->bTraversal = false;
		anim->bDashing = false;
		
		ClonePlayer->anim->bJumping = false;
		ClonePlayer->anim->bDoubleJumping = false;
		ClonePlayer->anim->bClimbing = false;
		ClonePlayer->anim->bTraversal = false;
		ClonePlayer->anim->bDashing = false;
		
		GetCharacterMovement()->GravityScale = 1.0f;
	}
	else
	{
		FHitResult OutHit;
		FVector HitLocation;
		FVector Normal;
		int index;
		
		if (bMoving && !bFailClimb && !bTraversal) bCanClimb = DetectWall(OutHit, HitLocation, Normal, index);
		
		if (bCanClimb)
		{
			if (index == 0)
			{
				float value = GetActorForwardVector().Dot(-OutHit.ImpactNormal);
				ClimbWall(FMath::RadiansToDegrees(FMath::Acos(value)));
			}
			else if (index == 1)
			{
				GetCharacterMovement()->Velocity = FVector::ZeroVector;
				GetCharacterMovement()->GravityScale = 0.0f;
				GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				bTraversal = true;
				bCanClimb = false;
				
				anim->bTraversal = true;
				
				ClonePlayer->anim->bTraversal = true;
				
				anim->Montage_Play(ClimbMontage);
				ClonePlayer->anim->Montage_Play(ClimbMontage);
			}
		}
	}
	
	if (bAdjustAnimaition)
	{
		SetActorLocation(GetActorLocation() + GetActorUpVector() * GetWorld()->GetDeltaSeconds() * 700.f);
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * GetWorld()->GetDeltaSeconds() * 100.f);
	} 

	if (bDashing)
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * GetWorld()->GetDeltaSeconds() * 700.f);
	}

	ConveyorBeltCheck(DeltaTime); 
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
	
	DOREPLIFETIME(ASplitPlayer, bDashing);
	
	DOREPLIFETIME(ASplitPlayer, bRunning);
	
	DOREPLIFETIME(ASplitPlayer, ClonePlayer); 

	DOREPLIFETIME(ASplitPlayer, CurPart); 
	DOREPLIFETIME(ASplitPlayer, MoveCheck); 
}

void ASplitPlayer::MoveAction(const FInputActionValue& Value)
{
	if (bClimbing) return;
	if (bDashing) return;
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

void ASplitPlayer::JumpServer_Implementation()
{
	GetCharacterMovement()->SetBase(nullptr); 
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
}

void ASplitPlayer::DoubleJumpMulti_Implementation()
{
	anim->bDoubleJumping = true;

	ClonePlayer->anim->bDoubleJumping = true;
}

void ASplitPlayer::JumpAction(const FInputActionValue& Value)
{
	if ((!bJumping || !bDoubleJumping) && !bDashing)
	{
		Jump(); 
	}
	JumpServer();
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
			Interact(II);
		}
	}
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
}

void ASplitPlayer::DashAction(const FInputActionValue& Value)
{
	if (!bRunning && !bDashing && !bClimbing)
	{
		DashServer();
	}
	
	if (GetCharacterMovement()->IsFalling())
	{
		
	}
}

void ASplitPlayer::RunMulti_Implementation(bool isRunning)
{
	anim->bRunning = isRunning;

	ClonePlayer->anim->bRunning = isRunning;
}

void ASplitPlayer::RunServer_Implementation()
{
	if (!bRunning && !bDashing && !bClimbing && !bJumping && bMoving)
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

void ASplitPlayer::RunAction(const FInputActionValue& Value)
{
	RunServer();
}

void ASplitPlayer::Die()
{ 
	if (CurPart == 6)
	{
		TArray<class AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), FoundActors);

		float Z;
		GetWorld()->GetParameterCollectionInstance(collection)->GetScalarParameterValue(FName(TEXT("World_Z")),Z);
		
		float distance = 999999.f;
		for (auto Actor : FoundActors)
		{
			if (Actor->GetActorLocation().Z > Z)
				if (distance > FVector::Dist(GetActorLocation(), Actor->GetActorLocation()))
				{
					distance = FVector::Dist(GetActorLocation(), Actor->GetActorLocation());
					SpawnTransform = Actor->GetActorTransform();
				}
		}
	}

	SetActorTransform(SpawnTransform);
}

bool ASplitPlayer::DetectWall(FHitResult& Out_Hit, FVector& HitLocation, FVector& Normal, int& index)	
{
	FHitResult OutHit;
	
	for (int detectIndex  = 0; detectIndex < 2; detectIndex++)
	{
		FVector tempLocation = MoveVectorDownward(MoveVectorUpward(GetActorLocation(), 40.f), detectIndex * 20.f);
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

	Out_Hit = OutHit;
	HitLocation=OutHit.Location;
	Normal = OutHit.Normal;
	return OutHit.bBlockingHit;
}

void ASplitPlayer::ClimbWall(float Value)
{
	if (Value > 5.0f) return;

	GetCharacterMovement()->GravityScale = 0.f;
	bClimbing = true;
	
	anim->bClimbing = true;
	
	ClonePlayer->anim->bClimbing = true;
	
	FHitResult OutHit;
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
	GetWorld(),
	MoveVectorUpward(GetActorLocation(), 70.f),
	MoveVectorUpward(GetActorLocation(), 90.f),
	10.f,
	(ETraceTypeQuery)ECC_Visibility,
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
		SetActorLocation(GetActorLocation() + GetActorUpVector() * GetWorld()->GetDeltaSeconds());
	}
	else if (bHit)
	{
		bFailClimb = true;
		bClimbing = false;
		
		anim->bClimbing = false;

		ClonePlayer->anim->bClimbing = false;
		
		GetCharacterMovement()->GravityScale = 1.f;
	}
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

void ASplitPlayer::CloneLocation_Implementation(FVector Location)
{
	ClonePlayer->SetActorLocation(Location - FVector(0, 0, 90));
}

void ASplitPlayer::SpawnClone_Implementation(FVector PlayerStart, FVector LocationOffset)
{
	FTransform CloneSpawnTransform = GetActorTransform();
	CloneSpawnTransform.SetLocation(PlayerStart + LocationOffset);
	
	SetActorLocation(PlayerStart);

	ClonePlayer = GetWorld()-> SpawnActor<AClonePlayer>(ClonePlayerFactory, CloneSpawnTransform);
}

void ASplitPlayer::Interact_Implementation(AInteractableActorBase* Actor)
{
	IInteractable::Execute_Interaction(Actor); 
} 

void ASplitPlayer::ChangePart()
{ 
	CurPart = int(GM->CurPart); 
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
