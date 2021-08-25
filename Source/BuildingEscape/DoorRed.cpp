// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorRed.h"

#include "GoldStatue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

#define OUT

// Sets default values for this component's properties
UDoorRed::UDoorRed()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/*
	TSubclassOf<AActor> ClassToFind;
	// Needs to be populated somehow (e.g. by exposing to blueprints as uproperty and setting it there
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);
	 */
}


// Called when the game starts
void UDoorRed::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	FindPressurePlate();
	FindAudioComponent();
}

// Called every frame
void UDoorRed::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	if (TotalMassOfActor() == MassToOpenDoors )
	{
		const TSubclassOf<AGoldStatue> ClassToFind = AGoldStatue::StaticClass();
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

		UE_LOG(LogTemp, Warning, TEXT("Found actors: %d"), FoundActors.Num());

		if (FoundActors.Num() == 0)
		{
			OpenDoor(DeltaTime);	
		}

		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		CloseDoor(DeltaTime);
}

void UDoorRed::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	CloseDoorSound = false;
	if (!Audio_DoorRed) return;
	if (!OpenDoorSound)
	{
		Audio_DoorRed->Play();
		OpenDoorSound = true;
	}
}

void UDoorRed::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenDoorSound = false;
	if (!Audio_DoorRed) return;;
	if (!CloseDoorSound)
	{
		//AudioComponent->VolumeMultiplier(0.5f);
		Audio_DoorRed->Play();
		CloseDoorSound = true;
	}
}

float UDoorRed::TotalMassOfActor()
{
	float TotalMass = 0.f;

	// Find all overlapping actors 
	TArray<AActor*> OverlappingActors;
	if (!Trigger_Red)
		return TotalMass;

	if (Trigger_Red)
		Trigger_Red->GetOverlappingActors(OUT OverlappingActors);

	// Add up their masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	
	return  TotalMass;
}

void UDoorRed::FindPressurePlate()
{
	if (!Trigger_Red)
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component, but no pressure plate set."), *GetOwner()->GetName());
}

void UDoorRed::FindAudioComponent()
{
	Audio_DoorRed = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!Audio_DoorRed)
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component"), *GetOwner()->GetName());
}


void UDoorRed::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OtherActor);
	if (PrimitiveComponent != nullptr)
	{
		TSubclassOf<AGoldStatue> ClassToFind;
		// Needs to be populated somehow (e.g. by exposing to blueprints as uproperty and setting it there
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

		UE_LOG(LogTemp, Warning, TEXT("Found Actor: %d"), FoundActors.Num());
		
		//UGameplayStatics::GetRealTimeSeconds(GetWorld());
		
		float DeltaTime = GetOwner()->GetWorld()->GetTimeSeconds();
		
		
	}
}