// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GoldStatue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Otolith.generated.h"

class UDoorRed;
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;

UCLASS()
class BUILDINGESCAPE_API AOtolith : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOtolith();

	TArray<FString> StrArr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UStaticMeshComponent* MeshComp = nullptr;

	

protected:
	// Move
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	UPROPERTY(EditAnywhere, Category = Trace)
	float Distance;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	int NumberOfStatues;
};
