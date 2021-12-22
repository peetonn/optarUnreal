// Fill out your copyright notice in the Description page of Project Settings.


#include "AInimal.h"
#include "..\Public\AInimal.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AAInimal::AAInimal()
	: CurrentState(AnimalState::STANDING)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAInimal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAInimal, CurrentState);
}

// Called when the game starts or when spawned
void AAInimal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAInimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAInimal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

