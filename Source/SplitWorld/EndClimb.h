// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EndClimb.generated.h"

/**
 * 
 */
UCLASS()
class SPLITWORLD_API UEndClimb : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
public:
	UPROPERTY() 
	class ASplitPlayer* player;
};
