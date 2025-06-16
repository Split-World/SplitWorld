// Fill out your copyright notice in the Description page of Project Settings.


#include "EndClimb.h"

#include "SplitPlayer.h"

void UEndClimb::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	player = Cast<ASplitPlayer>(MeshComp->GetOwner()); 

	if (IsValid(player))
	{
		player->EndClimbServer();
	}
}
