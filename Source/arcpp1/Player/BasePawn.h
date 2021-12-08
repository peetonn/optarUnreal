// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "BasePawn.generated.h"

UCLASS()
class ARCPP1_API ABasePawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// pawn transform replication
	UPROPERTY(ReplicatedUsing=OnRep_PawnTransform)
	FTransform PawnTransform;

	UFUNCTION(Server, Unreliable)
	void Server_SetPawnTransform(FTransform t);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	virtual void OnRep_PawnTransform();
};
