// Fill out your copyright notice in the Description page of Project Settings.


#include "XRAInimalsGameState.h"

void AXRAInimalsGameState::AddToPeopleCache(const AActor* Person)
{
	PeopleCache.Add(Person);
}

void AXRAInimalsGameState::RemoveFromPeopleCache(const AActor* Person)
{
	PeopleCache.Remove(Person);
}

const AActor* AXRAInimalsGameState::GetClosestPerson(const AActor* OtherActor)
{
	const AActor* BestPerson = nullptr;
	float MinSquaredDistance = 0.0;

	for (const AActor* Person : PeopleCache)
	{
		const float SquaredDistance = Person->GetSquaredDistanceTo(this);
		if (BestPerson == nullptr || SquaredDistance < MinSquaredDistance)
		{
			BestPerson = Person;
			MinSquaredDistance = SquaredDistance;
		}
	}
	return BestPerson;
}
