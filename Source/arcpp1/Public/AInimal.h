// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AInimal.generated.h"

UENUM(BlueprintType)
enum AnimalState
{
	STANDING	UMETA(DisplayName="Standing"),
	EATING		UMETA(DisplayName = "Eating"),
	SITTING		UMETA(DisplayName = "Sitting")
};

UCLASS()
class ARCPP1_API AAInimal : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAInimal();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadWrite)
	TEnumAsByte<AnimalState> CurrentState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// PERSONALITY TRAIT MULTIPLIERS

	UPROPERTY(Category = Personality, EditAnywhere, BlueprintReadOnly)
	float HungerMultiplier = 1.0;

	UPROPERTY(Category = Personality, EditAnywhere, BlueprintReadOnly)
	float ActiveMultiplier = 1.0;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
