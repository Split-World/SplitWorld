// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "WalkSoundNS.generated.h"

/**
 * 
 */
UCLASS()
class SPLITWORLD_API UWalkSoundNS : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
public:
	UPROPERTY() 
	class ASplitPlayer* player;
};
