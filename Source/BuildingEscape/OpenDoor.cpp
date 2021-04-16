// Copyright 2020


#include "OpenDoor.h"
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
	TargetYaw += InitialYaw; // Set target Yaw as initial Yaw + 90 degrees

	if (!PressurePlate)
	{
		FString ActorName = GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor Component attached but no PressurePlate set."), *ActorName);
	}

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpen))
	{
		OpenDoor(DeltaTime);
	}

}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * 1.f); // Get current Yaw each frame
	FRotator DoorRotation = GetOwner()->GetActorRotation(); // Get the current FRotator each frame
	DoorRotation.Yaw = CurrentYaw; // Interpolate between current and target Yaw
	GetOwner()->SetActorRotation(DoorRotation); // Set interpolated value as new Yaw value

}