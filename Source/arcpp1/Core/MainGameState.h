// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MainGameState.generated.h"

/**
 * Custom game state class.
 * Game State class is used for game-wide properties, like current loaded level,
 * other game state variables like total score, etc.
 * Game State is replicated to all clients.
 * Game State is created when client logs in and destoyed on logout.
 */
UCLASS()
class ARCPP1_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

public:


};
