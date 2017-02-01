// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}


// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Component NOT Found!!"));
	}
}

// Look for attached Input Component
void UGrabber::SetupInputComponent()
{
	PlayerInput = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PlayerInput)
	{
		PlayerInput->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		PlayerInput->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component NOT Found!!"));
	}
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// if the physics handle is attached and grabbed something
	if(PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());	// move object that we're holding
	}
}

void UGrabber::Grab()
{
	if (PhysicsHandle) 
	{
		// LINE TRACE and see if we reach any actors with physics body collision channel set
		auto HitResult = GetFirstPhysicsBodyInReach();
		auto ComponentToGrab = HitResult.GetComponent();	// gets the mesh in our case
		auto ActorHit = HitResult.GetActor();

		// if we hit something then attach a physics handle
		if (ActorHit)
		{
		
			PhysicsHandle->GrabComponentAtLocationWithRotation(		// attach physics handle
				ComponentToGrab,
				NAME_None,	// no bones needed
				ComponentToGrab->GetOwner()->GetActorLocation(),
				ComponentToGrab->GetOwner()->GetActorRotation()
			);
		}
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle) 
	{ 
		PhysicsHandle->ReleaseComponent();
	}
}

// Line-Trace (ray-cast) out to reach distance
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	
	// Check if we actually hit a PhysicsBody type object
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return Hit;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerPosition;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerPosition,
		OUT PlayerRotation);
	return PlayerPosition;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerPosition;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerPosition,
		OUT PlayerRotation);
	return PlayerPosition + PlayerRotation.Vector()*Reach;
}

