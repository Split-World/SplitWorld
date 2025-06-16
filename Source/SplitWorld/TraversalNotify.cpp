// Fill out your copyright notice in the Description page of Project Settings.


#include "TraversalNotify.h"

#include "SplitPlayer.h"

void UTraversalNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	player = Cast<ASplitPlayer>(MeshComp->GetOwner());

	if (IsValid(player))
	{
		player->StartTraversalServer();
	}
}

void UTraversalNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (IsValid(player))
	{
		player->EndTraversalServer();
	}
}