// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MultiplayerPawnComponent.h"
#include "ARBasePawn.generated.h"

/**
 * Custom class for AR Pawn
 */
UCLASS()
class ARCPP1_API AARBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AARBasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly)
	UMultiplayerPawnComponent* MultiplayerPawnComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
