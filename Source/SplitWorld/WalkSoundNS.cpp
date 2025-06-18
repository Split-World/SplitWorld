// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkSoundNS.h"
#include "SplitPlayer.h"

void UWalkSoundNS::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	player = Cast<ASplitPlayer>(MeshComp->GetOwner());

	if (IsValid(player))
	{
		player->MoveSoundServer();
	}
}
