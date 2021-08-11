// Fill out your copyright notice in the Description page of Project Settings.


#include "Otolith.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AOtolith::AOtolith()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	MeshComp->SetupAttachment(RootComponent);
}

// Called every frame
void AOtolith::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOtolith::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AOtolith::Jump);

	// Move
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AOtolith::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AOtolith::MoveRight);
	
	// Look
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	
}


void AOtolith::MoveForward(float AxisValue)
{
	if ( (Controller != nullptr) && (AxisValue != 0.0f) )
	{
		AddMovementInput(GetActorForwardVector() * AxisValue);
	}
}

void AOtolith::MoveRight(float AxisValue)
{
	if ( (Controller != nullptr) && (AxisValue != 0.0f) )
	{
		AddMovementInput(GetActorRightVector() * AxisValue);
	}
}


