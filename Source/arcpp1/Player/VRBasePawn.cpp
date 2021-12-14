// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VRBasePawn.h"

AVRBasePawn::AVRBasePawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetReplicateMovement(true);

    MultiplayerPawnComponent = CreateDefaultSubobject<UMultiplayerPawnComponent>(TEXT("MultiplayerPawnComponent"));
    AddOwnedComponent(MultiplayerPawnComponent);
}

// Called every frame
void AVRBasePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
