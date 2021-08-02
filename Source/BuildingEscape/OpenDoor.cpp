// Copyright 2020


#include "OpenDoor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenYaw += InitialYaw;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor Component attached but no PressurePlate set."), *GetOwner()->GetName());
	}

	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpen)) // Open door if actor is overlapping pressure plate
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("Door was last opened at %f"), DoorLastOpened);
	}
	else if (AutoClose && ((DoorCloseDelay + DoorLastOpened) < GetWorld()->GetTimeSeconds())) // close door if actor not overlapping
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) // Rotate door to open yaw
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenYaw, DeltaTime * DoorOpenSpeed); // Get current Yaw each frame
	FRotator DoorRotation = GetOwner()->GetActorRotation(); // Get the current FRotator each frame
	DoorRotation.Yaw = CurrentYaw; // Interpolate between current and target Yaw
	GetOwner()->SetActorRotation(DoorRotation); // Set interpolated value as new Yaw value
}

void UOpenDoor::CloseDoor(float DeltaTime) // Rotate door to initial yaw
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed); // Get current Yaw each frame
	FRotator DoorRotation = GetOwner()->GetActorRotation(); // Get the current FRotator each frame
	DoorRotation.Yaw = CurrentYaw; // Interpolate between current and target Yaw
	GetOwner()->SetActorRotation(DoorRotation); // Set interpolated value as new Yaw value
}