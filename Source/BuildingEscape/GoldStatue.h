// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "GoldStatue.generated.h"


class AOtolith;
UCLASS()
class BUILDINGESCAPE_API AGoldStatue : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AGoldStatue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Item)
	float Time;

	UPROPERTY(EditAnywhere, Category = Collider)
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnyWhere, Category = Mesh)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnyWhere, Category = StatueMovement)
	FVector RotSpeed;

	UPROPERTY(EditAnyWhere, Category = StatueMovement)
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger capsule")
	class UCapsuleComponent* TriggerCapsule;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= StatueAudio)
	UAudioComponent* Audio_PickStatue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= StatueAudio)
	USoundCue* Audio_Cue = nullptr;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
};
