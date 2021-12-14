// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/MultiplayerPawnComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "VRBasePawn.generated.h"

/**
 *
 */
UCLASS()
class ARCPP1_API AVRBasePawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	AVRBasePawn();

	UPROPERTY(BlueprintReadOnly)
	UMultiplayerPawnComponent* MultiplayerPawnComponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
};
