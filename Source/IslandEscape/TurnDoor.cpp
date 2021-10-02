// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnDoor.h"
#include "Components/AudioComponent.h"

#define OUT

// Sets default values for this component's properties
UTurnDoor::UTurnDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTurnDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	FindPressurePlate();
	FindAudioComponent();
}

// Called every frame
void UTurnDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);
	
	if (TotalMassOfActor() == MassToOpenDoors)
	{
		OpenDoor(DeltaTime);

		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		CloseDoor(DeltaTime);
}

void UTurnDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	CloseDoorSound = false;
	if (!AudioDoor) return;
	if (!OpenDoorSound)
	{
		AudioDoor->Play();
		OpenDoorSound = true;
	}
}

void UTurnDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenDoorSound = false;
	if (!AudioDoor) return;;
	if (!CloseDoorSound)
	{
		//AudioComponent->VolumeMultiplier(0.5f);
		AudioDoor->Play();
		CloseDoorSound = true;
	}
}

float UTurnDoor::TotalMassOfActor() const
{
	float TotalMass = 0.f;

	// Find all overlapping actors 
	TArray<AActor*> OverlappingActors;
	if (!TriggerDoor)
		return TotalMass;

	if (TriggerDoor)
		TriggerDoor->GetOverlappingActors(OUT OverlappingActors);

	// Add up their masses
	for (const AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	//UE_LOG(LogTemp, Warning, TEXT("TotalMass: %f"), TotalMass)
	
	return  TotalMass;
}

void UTurnDoor::FindPressurePlate()
{
	if (!TriggerDoor)
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component, but no pressure plate set."), *GetOwner()->GetName());
}

void UTurnDoor::FindAudioComponent()
{
	AudioDoor = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioDoor)
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component"), *GetOwner()->GetName());
}