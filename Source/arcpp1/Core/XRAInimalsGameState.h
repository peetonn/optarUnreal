// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "XRAInimalsGameState.generated.h"

/**
 * Custom game state class.
 * Game State class is used for game-wide properties, like current loaded level,
 * other game state variables like total score, etc.
 * Game State is replicated to all clients.
 * Game State is created when client logs in and destoyed on logout.
 */
UCLASS()
class ARCPP1_API AXRAInimalsGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// An array of people, used to more efficiently track the people in the game
	// The person actors must register themselves to be tracked
	TArray<const AActor*> PeopleCache;

	// Store the person in the PeopleCache
	UFUNCTION(BlueprintCallable)
	void AddToPeopleCache(const AActor* Person);

	// Remove the person from the PeopleCache
	UFUNCTION(BlueprintCallable)
	void RemoveFromPeopleCache(const AActor* Person);

	// Returns the closest person to the other actor
	UFUNCTION(BlueprintCallable)
	const AActor* GetClosestPerson(const AActor* OtherActor);
};
