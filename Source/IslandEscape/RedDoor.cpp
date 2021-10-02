// Fill out your copyright notice in the Description page of Project Settings.


#include "RedDoor.h"
#include "GoldStatue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

#define OUT

class AGoldStatue;
// Sets default values for this component's properties
URedDoor::URedDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void URedDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	FindPressurePlate();
	FindAudioComponent();
}

// Called every frame
void URedDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);
	
	if (TotalMassOfActor() == MassToOpenDoors)
	{
		const TSubclassOf<AGoldStatue> ClassToFind = AGoldStatue::StaticClass();
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

		//UE_LOG(LogTemp, Warning, TEXT("Found actors: %d"), FoundActors.Num());

		if (FoundActors.Num() == 0)
		{
			OpenDoor(DeltaTime);	
		}

		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		CloseDoor(DeltaTime);

}

void URedDoor::OpenDoor(float DeltaTime)
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

void URedDoor::CloseDoor(float DeltaTime)
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

float URedDoor::TotalMassOfActor() const
{
	float TotalMass = 0.f;

	// Find all overlapping actors 
	TArray<AActor*> OverlappingActors;
	if (!TriggerDoor)
		return TotalMass;

	if (TriggerDoor)
		TriggerDoor->GetOverlappingActors(OUT OverlappingActors);

	//UE_LOG(LogTemp, Warning, TEXT("OverlappingActors: %f"), OverlappingActors.Num())
	
	// Add up their masses
	for (const AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	//UE_LOG(LogTemp, Warning, TEXT("TotalMass: %f"), TotalMass)
	
	return  TotalMass;
}

void URedDoor::FindPressurePlate()
{
	if (!TriggerDoor)
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component, but no pressure plate set."), *GetOwner()->GetName());
}

void URedDoor::FindAudioComponent()
{
	AudioDoor = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioDoor)
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component"), *GetOwner()->GetName());
}

void URedDoor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OtherActor);
	if (PrimitiveComponent != nullptr)
	{
		const TSubclassOf<AGoldStatue> ClassToFind;		
		// Needs to be populated somehow (e.g. by exposing to blueprints as uproperty and setting it there
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

		//UE_LOG(LogTemp, Warning, TEXT("Found Actor: %d"), FoundActors.Num());
		
		float DeltaTime = GetOwner()->GetWorld()->GetTimeSeconds();
	}
}

