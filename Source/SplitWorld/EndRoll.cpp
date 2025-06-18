// Fill out your copyright notice in the Description page of Project Settings.


#include "EndRoll.h"
#include "SplitPlayer.h"

void UEndRoll::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	player = Cast<ASplitPlayer>(MeshComp->GetOwner()); 

	if (IsValid(player))
	{
		player->EndRollServer();
	}
}
