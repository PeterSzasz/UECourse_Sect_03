// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if(PhysicsHandle)
	{
		// Physics Handle found
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics Component NOT Found!!"));
	}
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Get the player view point and rotation
	FVector PlayerPosition;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(
		OUT PlayerPosition,
		OUT PlayerRotation);

	// Log pos and rot for testing
	/*UE_LOG(LogTemp, Warning, TEXT("Player Position: %s Rotation: %s"), 
		*PlayerPosition.ToString(), 
		*PlayerRotation.ToString()
	);*/

	FVector LineTraceEnd = PlayerPosition + PlayerRotation.Vector()*Reach;
	// Ray-cast out to reach distance

	// Draw a red trace to visualise where and what we can reach
	DrawDebugLine(
		GetWorld(),
		PlayerPosition,
		LineTraceEnd,
		FColor(255,0,0),
		false,
		0.f,
		0.f,
		10.f
		);

	// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;

	// Check if we actually hit a PhysicsBody type object
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerPosition,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// See what we hit, if we hit
	AActor* ActorHit = Hit.GetActor();
	if(ActorHit){
		UE_LOG(LogTemp, Warning, TEXT("We hit: %s"),*ActorHit->GetName());
	}

}

