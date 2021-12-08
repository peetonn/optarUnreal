// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePawn.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	// bReplicateMovement = true;
	// SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABasePawn, PawnTransform, COND_SimulatedOnly); //COND_SkipOwner);
}

void ABasePawn::OnRep_PawnTransform()
{
	// update this pawn transform
	FHitResult SweepHitResult;
	SetActorTransform(PawnTransform, false, &SweepHitResult, ETeleportType::None);
}

void ABasePawn::Server_SetPawnTransform_Implementation(FTransform t)
{
	PawnTransform = t;
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		// only update if change is significant
		if (!GetActorTransform().Equals(PawnTransform, 0.05))
		{
			PawnTransform = GetActorTransform();
			Server_SetPawnTransform(GetActorTransform());
		}
	}
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
