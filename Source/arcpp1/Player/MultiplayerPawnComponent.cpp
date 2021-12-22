// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MultiplayerPawnComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UMultiplayerPawnComponent::UMultiplayerPawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UMultiplayerPawnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UMultiplayerPawnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMultiplayerPawnComponent, PawnTransform);
	// DOREPLIFETIME_CONDITION(ABasePawn, PawnTransform, COND_SkipOwner);
}

void UMultiplayerPawnComponent::OnRep_PawnTransform()
{
	// update this pawn transform
	FHitResult SweepHitResult;

	AActor* owner = GetOwner();
	owner->SetActorTransform(PawnTransform, false, &SweepHitResult, ETeleportType::None);
}

void UMultiplayerPawnComponent::Server_SetPawnTransform_Implementation(FTransform t)
{
	PawnTransform = t;
	OnRep_PawnTransform(); // why is this needed? shouldn't replication for PawnTransform kick in?
}

// Called every frame
void UMultiplayerPawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* owner = GetOwner();

	if (owner->GetLocalRole() >= ROLE_AutonomousProxy) //HasAuthority())
	{
		FTransform t = owner->GetActorTransform();
		// only update if change is significant
		if (!FTransform::AreRotationsEqual(t, PawnTransform, 0.01) ||
			!FTransform::AreTranslationsEqual(t, PawnTransform, 0.05))
		{
			PawnTransform = t;
			Server_SetPawnTransform(t);
		}
	}
}
