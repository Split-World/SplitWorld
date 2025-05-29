// Fill out your copyright notice in the Description page of Project Settings.


#include "EndClimb.h"

#include "SplitPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEndClimb::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	auto player = Cast<ASplitPlayer>(MeshComp->GetOwner());

	if (player)
	{
		player->GetCharacterMovement()->GravityScale = 1.0f;
		player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		player->SetActorLocation(player->GetActorLocation() + FVector::UpVector * player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2);
	}
}
