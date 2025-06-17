// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractableActorBase.generated.h"

UENUM()
enum class EFunctionType : uint8
{
	ShowInteraction,
	HideInteraction,
	ChangeIMG,
	ActiveKey,
	DeactiveKey 
};

UCLASS()
class SPLITWORLD_API AInteractableActorBase : public AActor, public IInteractable  
{
	GENERATED_BODY()
	
public: 
	AInteractableActorBase();
	UFUNCTION()
	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Interaction_Implementation() override;

	UPROPERTY(EditAnywhere, Replicated) 
	int Idx; 

protected: 
	UFUNCTION()
	void OnVisibleUIRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); 
	UFUNCTION() 
	void OnVisibleUIRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnInteractableRangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() 
	void OnInteractableRangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex); 
	UFUNCTION(NetMulticast, Reliable)
	void Multi_ExecuteFunction(EFunctionType Type, bool Value); 
	UFUNCTION(NetMulticast, Reliable)
	void Multi_SetVisibility(); 
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp; 
	UPROPERTY(EditAnywhere)
	class USphereComponent* VisibleUIRangeComp;
	UPROPERTY(EditAnywhere)
	class USphereComponent* InteractableRangeComp; 
	UPROPERTY(EditAnywhere)
	class UWidgetComponent* InteractionWidgetComp;

	bool bCanInteraction; 
	
private: 
	UPROPERTY(EditAnywhere)
	class UInteractionWidget* InteractionWidget;

	UPROPERTY(EditAnywhere)
	class AFirstCamera* Camera;
	UPROPERTY(EditAnywhere)
	int Map; 

};
