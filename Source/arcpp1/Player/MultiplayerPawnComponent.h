// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MultiplayerPawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCPP1_API UMultiplayerPawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMultiplayerPawnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// pawn transform replication
	UPROPERTY(ReplicatedUsing = OnRep_PawnTransform)
	FTransform PawnTransform;

	UFUNCTION(Server, Unreliable)
	void Server_SetPawnTransform(FTransform t);

private:
	UFUNCTION()
	virtual void OnRep_PawnTransform();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
