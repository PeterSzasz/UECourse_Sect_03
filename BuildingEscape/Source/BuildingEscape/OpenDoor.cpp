// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if(GetTotalMassOfActorOnPlate() >= TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorOnPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	// Find all the overlapping actors

	if(!PressurePlate){return 0.f;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	// Iterate through them adding their masses
	for (const auto* Actors : OverlappingActors)
	{
		TotalMass += Actors->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
