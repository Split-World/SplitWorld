// Fill out your copyright notice in the Description page of Project Settings.


#include "EndDash.h"
#include "SplitPlayer.h"

void UEndDash::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	player = Cast<ASplitPlayer>(MeshComp->GetOwner());

	if (IsValid(player))
	{
		player->bDashing = true;
	}
}

void UEndDash::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (IsValid(player))
	{
		player->bDashing = false;
	}
}