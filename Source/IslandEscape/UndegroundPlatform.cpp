// Fill out your copyright notice in the Description page of Project Settings.


#include "UndegroundPlatform.h"

// Sets default values for this component's properties
UUndegroundPlatform::UUndegroundPlatform()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Speed = 10.0f;
    Time = 0.0f;
	Distant = 1500;
}

// Called when the game starts
void UUndegroundPlatform::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UUndegroundPlatform::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector Pos = GetOwner()->GetActorLocation();
    Pos.Y = Distant * FMath::Sin(Time);
    Time += DeltaTime;
    GetOwner()->SetActorLocation(Pos);
}

