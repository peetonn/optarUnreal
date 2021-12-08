// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePawn.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasePawn, PawnTransform);
	// DOREPLIFETIME_CONDITION(ABasePawn, PawnTransform, COND_SkipOwner);
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
	OnRep_PawnTransform(); // why is this needed? shouldn't replication for PawnTransform kick in?
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() >= ROLE_AutonomousProxy) //HasAuthority())
	{
		FTransform t = GetActorTransform();
		// only update if change is significant
		if (!FTransform::AreRotationsEqual(t, PawnTransform, 0.01) ||
			!FTransform::AreTranslationsEqual(t, PawnTransform, 0.05))
		{
			PawnTransform = t;
			Server_SetPawnTransform(t);
		}
	}
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
