// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/TriggerVolume.h"
#include "DoorRed.generated.h"

class AGoldStatue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UDoorRed : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDoorRed();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActor();
	void FindAudioComponent();
	void FindPressurePlate();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Door)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Door)
	float DoorCloseDelay = .5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Door)
	float DoorOpenSpeed = .8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Door)
	float DoorCloseSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Door)
	float MassToOpenDoors = 150.f;

	UPROPERTY(EditAnywhere, Category= Door)
	ATriggerVolume* Trigger_Red = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Door)
	UAudioComponent* Audio_DoorRed = nullptr;

private:
	float InitialYaw;
	float CurrentYaw;

	float DoorLastOpened = 0.f;
};
