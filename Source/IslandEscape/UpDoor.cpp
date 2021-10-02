// Fill out your copyright notice in the Description page of Project Settings.


#include "UpDoor.h"
#include "Components/AudioComponent.h"

#define OUT

// Sets default values for this component's properties
UUpDoor::UUpDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UUpDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorLocation().Z;
	CurrentYaw = InitialYaw;
	OpenDistant += InitialYaw;

	FindPressurePlate();
	FindAudioComponent();
}

// Called every frame
void UUpDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorLocation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Z);
	
	if (TotalMassOfActor() == MassToOpenDoors)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Total Mass ==! %f"), TotalMassOfActor())
		
		OpenDoor(DeltaTime);

		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		CloseDoor(DeltaTime);
}

void UUpDoor::OpenDoor(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Z is: %f"), GetOwner()->GetActorLocation().Z);
	
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenDistant, DeltaTime * DoorOpenSpeed);
	FVector DoorDisplacement = GetOwner()->GetActorLocation();
	DoorDisplacement.Z = CurrentYaw;
	GetOwner()->SetActorLocation(DoorDisplacement);

	CloseDoorSound = false;
	if (!AudioDoor) return;
	if (!OpenDoorSound)
	{
		AudioDoor->Play();
		OpenDoorSound = true;
	}
}

void UUpDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FVector DoorDisplacement = GetOwner()->GetActorLocation();
	DoorDisplacement.Z = CurrentYaw;
	GetOwner()->SetActorLocation(DoorDisplacement);

	OpenDoorSound = false;
	if (!AudioDoor) return;;
	if (!CloseDoorSound)
	{
		//AudioComponent->VolumeMultiplier(0.5f);
		AudioDoor->Play();
		CloseDoorSound = true;
	}
}

float UUpDoor::TotalMassOfActor() const
{
	float TotalMass = 0.f;

	// Find all overlapping actors 
	TArray<AActor*> OverlappingActors;
	if (!TriggerDoor)
		return TotalMass;

	if (TriggerDoor)
		TriggerDoor->GetOverlappingActors(OUT OverlappingActors);

	//UE_LOG(LogTemp, Warning, TEXT("Works!"))
	
	// Add up their masses
	for (const AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	
	return  TotalMass;
}

void UUpDoor::FindPressurePlate()
{
	if (!TriggerDoor)
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component, but no pressure plate set."), *GetOwner()->GetName());
}

void UUpDoor::FindAudioComponent()
{
	AudioDoor = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioDoor)
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component"), *GetOwner()->GetName());
}
