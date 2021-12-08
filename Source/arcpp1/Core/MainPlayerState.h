// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

/**
 * Custom player state class.
 * Use it for storing game-wide player state values, such as player name,
 * various flags, etc. Player state is replicated to all client.
 * Player State destroyed on logout, created on login.
 */
UCLASS()
class ARCPP1_API AMainPlayerState : public APlayerState
{
    AMainPlayerState();
    AMainPlayerState(const FObjectInitializer& ObjectInitializer);
    ~AMainPlayerState();

	GENERATED_BODY()

public:



};
